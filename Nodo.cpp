#include "Nodo.h"

Nodo::Nodo(unsigned int maxNN) {
	vDistancia.reserve(maxNN);
	vVecino.reserve(maxNN);
#ifndef _DEBUG
	omp_init_lock(&lock_var);
#endif
}

unsigned int Nodo::blind_insert(const float ds, const unsigned int idx) {
	unsigned int n1 = vDistancia.size()-1;
	while(n1 > 0 && ds < vDistancia[n1-1]) {
		vDistancia[n1] = vDistancia[n1-1];
		vVecino[n1] = vVecino[n1-1];
		n1--;
	}
	vDistancia[n1] = ds;
	vVecino[n1] = idx;
	return n1;
}

unsigned int Nodo::safe_insert(const float ds, const unsigned int idx) {
	for(unsigned int n1=0; n1<vVecino.size(); n1++) {
		if(vVecino[n1] == idx)
			return vDistancia.size();
	}

	return blind_insert(ds, idx);
}

void Nodo::append(const float ds, const unsigned int idx) {
	vDistancia.resize( vDistancia.size() + 1 );
	vVecino.resize( vVecino.size() + 1 );
	blind_insert(ds, idx);
}

void Nodo::lock() {
#ifndef _DEBUG
	omp_set_lock(&lock_var);
#endif
}

bool Nodo::test_lock() {
#ifdef _DEBUG
	return true;
#else
	return omp_test_lock(&lock_var);
#endif
}

void Nodo::unlock() {
#ifndef _DEBUG
	omp_unset_lock(&lock_var);
#endif
}

void Nodo::sort() {
	unsigned int K = vVecino.size(), i, j, prev;
	std::vector<unsigned int> order(K);
	std::vector<unsigned int> done(K);
	std::fill(done.begin(), done.end(), false);
	std::iota(order.begin(), order.end(), 0);
	std::sort(order.begin(), order.end(),
			[&](int i1, int i2) { return vDistancia[i1] < vDistancia[i2]; });
	for(i=0; i<K; i++) {
		if (done[i])
			continue;
		done[i] = true;
		prev = i;
		j = order[i];
		while (i != j) {
			std::swap(vVecino[prev], vVecino[j]);
			std::swap(vDistancia[prev], vDistancia[j]);
			done[j] = true;
			prev = j;
			j = order[j];
		}
	}
	// std::sort(vVecino.begin(), vVecino.end(),
			// [&](int i1, int i2) { return vDistancia[i1] < vDistancia[i2]; });
	// std::sort(vDistancia.begin(), vDistancia.end());
}
