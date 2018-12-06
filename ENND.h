#ifndef ENNDESCENT_H
#define ENNDESCENT_H
#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>
#include <iostream>
#include "timeManager.h"
#include "Nodo.h"
#include "flagVector.h"

#ifndef __APPLE__
#include <omp.h>
// [[Rcpp::plugins(openmp)]]
#endif

typedef float (*fptr)(unsigned int, unsigned int);
using std::vector;

class ENNDescent {
private:
	fptr distance;
	unsigned int N;
	unsigned int K;
	unsigned int rK;
	vector<Nodo> vNodo;
	timeManager tman;
	unsigned int unirNodos(const unsigned int i, const unsigned int k, unsigned int& n1);
	void processNode(const unsigned int i, flagVector& to_avoid, unsigned int& cambios);
public:
	ENNDescent(unsigned int N, unsigned int K, fptr distance, unsigned int OS);
	ENNDescent(unsigned int N, unsigned int K, fptr distance, unsigned int OS,
			vector<unsigned int>& idx, vector<unsigned int>& ptr);
	unsigned int iterate();

	unsigned int operator() (unsigned int i, unsigned int n1) const;
	void toSparseMatrix(vector<unsigned int>& idxs, vector<unsigned int>& ptrs, vector<float>& vls);

	void checkGraph();
};
#endif

