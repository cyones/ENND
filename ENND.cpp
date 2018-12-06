#include "ENND.h"

ENNDescent::ENNDescent(unsigned int N, unsigned int _K, fptr distance, unsigned int OS) :
	distance(distance), N(N), K(_K + OS), rK(_K), vNodo(N, Nodo(K)), tman(N, K, 8) {
	unsigned int i, j, n=0;
	float ds;
	// std::vector<float> dst(N);
	// std::vector<unsigned int> mapper(N);
	// std::iota(mapper.begin(), mapper.end(), 0);
	// std::generate(dst.begin(), dst.end(), [&]{ return distance(0,n++);});
	// std::sort(mapper.begin(), mapper.end(), [&](int i1, int i2) { return dst[i1] < dst[i2]; } );
	for(i = 0; i < N; i++) {
		for(n = 0; n < K; n++) {
			j = (i+n+1) % N;
			// ds = distance(mapper[i], mapper[j]);
			// vNodo[mapper[i]].append(ds, mapper[j]);
			ds = distance(i, j);
			vNodo[i].append(ds, j);
		}
	}
}

ENNDescent::ENNDescent(unsigned int N, unsigned int _K, fptr distance, unsigned int OS,
		vector<unsigned int>& idx, vector<unsigned int>& ptr) :
	distance(distance), N(N), K(_K + OS), rK(_K), vNodo(N, Nodo(K)), tman(N, K, 8) {
	unsigned int i, j, n, nK, mnn;
	for(i=0; i<N; i++) {
		Nodo& ni = vNodo[i];
		nK = std::min(K, ptr[i+1] - ptr[i]);
		for(n=0; n<nK; n++) {
			j = idx[ptr[i] + n];
			ni.append(distance(i, j), j);
		}
		mnn = K - nK;
		while(mnn) {
			j = rand() % N;
			if(j != i && std::find(ni.vVecino.begin(), ni.vVecino.end(), j) == ni.vVecino.end()) {
				ni.append(distance(i, j), j);
				mnn--;
			}
		}
		ni.sort();
	}
}


unsigned int ENNDescent::unirNodos(const unsigned int i, const unsigned int k, unsigned int& n1) {
	unsigned int idx;
	unsigned int modification = 0;

	float ds = distance(i,k);

	if(ds < vNodo[i].vDistancia.back()) {
		idx = vNodo[i].blind_insert(ds, k);
		if(idx <=  n1) n1++;
		tman.added(i, idx);
		modification++;
	}
	if(ds < vNodo[k].vDistancia.back()) {
		idx = vNodo[k].safe_insert(ds, i);
		if(idx < K) {
			modification++;
			tman.added(k, idx);
		}
	}
	return modification;
}

void ENNDescent::processNode(const unsigned int i, flagVector& to_avoid, unsigned int& cambios) {
	if(!vNodo[i].test_lock())
		return;
	if(!tman.needs_procesing(i)) {
		vNodo[i].unlock();
		return;
	}
	unsigned int j, k, n1, n2;
	to_avoid.clear_all();
	vector<unsigned int>& vvi = vNodo[i].vVecino;
	to_avoid.mark(i);
	for(n1 = 0; n1 < K; n1++)
		to_avoid.mark(vvi[n1]);
	for(n1 = 0; n1 < K; n1++) {
		j = vvi[n1];
		if(!vNodo[j].test_lock())
			continue;
		if(!tman.needs_procesing(i, n1, j)) {
			vNodo[j].unlock();
			continue;
		}
		vector<unsigned int>& vvj = vNodo[j].vVecino;
		for(n2 = 0; n2 < K; n2++) {
			k = vvj[n2];
			if(to_avoid.is_marked(k))
				continue;
			to_avoid.mark(k);
			if(vNodo[k].test_lock()) {
				if(tman.needs_procesing(i,n1,j,n2))
					cambios += unirNodos(i, k, n1);
				vNodo[k].unlock();
			}
		}
		tman.checked(i,n1);
		vNodo[j].unlock();
	}
	vNodo[i].unlock();
}

unsigned int ENNDescent::iterate() {
	unsigned int cambios = 0;
#pragma omp parallel reduction(+:cambios)
	{
		flagVector to_avoid(N);
#pragma omp for schedule(static)
		for(unsigned int i = 0; i < N; i++)
			processNode(i, to_avoid, cambios);
	}
	tman.iterate();
	return cambios;
}

unsigned int ENNDescent::operator() (unsigned int i, unsigned int n1) const {
	return vNodo[i].vVecino[n1];
}

void ENNDescent::toSparseMatrix(vector<unsigned int>& idxs, vector<unsigned int>& jdxs, vector<float>& vals) {
	unsigned int i, n1;
	idxs.resize(N * rK, 0);
	vals.resize(N * rK, 0);
	jdxs.resize(N * rK, 0);
	for(i=0; i<N; i++) {
		for(n1=0; n1 < rK; n1++) {
			idxs[i * rK + n1] = vNodo[i].vVecino[n1] + 1;
			jdxs[i * rK + n1] = i + 1;
			vals[i * rK + n1] = vNodo[i].vDistancia[n1];
		}
	}
}

// void ENNDescent::checkGraph() {
	// unsigned int repetido=0, loop=0, locked=0, unsorted=0;
	// unsigned int i, j;
	// for(i=0; i<N; i++) {
		// locked += !vNodo[i].test_lock();
		// vNodo[i].unlock();
		// loop += vNodo[i].vVecino[0] == i;
		// for(j=1; j<vNodo[i].vVecino.size(); j++) {
			// repetido += vNodo[i].vVecino[j] == vNodo[i].vVecino[j-1];
			// loop += vNodo[i].vVecino[j] == i;
			// unsorted += vNodo[i].vDistancia[j] < vNodo[i].vDistancia[j-1];
		// }
	// }
	// if(repetido) std::cout<<" - Repetidos: "<<repetido<<std::endl;
	// if(loop) std::cout<<" - Loops: "<<loop<<std::endl;
	// if(loop) std::cout<<" - Unsorted: "<<unsorted<<std::endl;
	// if(locked) std::cout<<" - locked: "<<locked<<std::endl;
// }

