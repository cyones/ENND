#include "timeManager.h"

timeManager::timeManager (unsigned int N, unsigned int K, unsigned char iter_to_freeze) :
	edge_added(N*K, 0), edge_checked(N*K, 0), last_change(N, 0),
	iteration(1), N(N), K(K), iter_to_freeze(iter_to_freeze) {
	iter_to_freeze = std::min(iter_to_freeze, (unsigned char)49);
}

void timeManager::iterate ( ) {
	if(iteration < 250)
		iteration++;
	else {
		// TODO: probar esta parte
		iteration = 51;
		for(unsigned int i=0; i<edge_added.size(); i++) {
			edge_added[i] = (edge_added[i] < 200? 0 : edge_added[i]-200);
			edge_checked[i] = (edge_checked[i] < 200? 0 : edge_checked[i]-200);
		}
		for(unsigned int i=0;i<last_change.size();i++)
			last_change[i] = (last_change[i] < 200? 0 : last_change[i]-200);
	}
}

void timeManager::checked (unsigned int i, unsigned int n1) {
	edge_checked[i*K + n1] = iteration;
}

void timeManager::added (unsigned int i, unsigned int n1) {
	unsigned int idx = i*K + K-1;
	n1 += i*K;
	while(idx > n1) {
		edge_added[idx] = edge_added[idx-1];
		edge_checked[idx] = edge_checked[idx-1];
		idx--;
	}
	edge_added[n1] = iteration;
	edge_checked[n1] = 0;
	last_change[i] = iteration;
}

bool timeManager::needs_procesing (unsigned int i) {
	return iteration - last_change[i] < iter_to_freeze;
}

bool timeManager::needs_procesing (unsigned int i, unsigned int n1, unsigned int j) {
	return last_change[j] >= edge_checked[i*K + n1];
}

bool timeManager::needs_procesing (unsigned int i, unsigned int n1, unsigned int j, unsigned int n2) {
	return edge_checked[i*K + n1] <= edge_added[j*K + n2];
}

