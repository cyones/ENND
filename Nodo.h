#ifndef NODO_H
#define NODO_H
#include <vector>
#include <algorithm>
#include <numeric>
#include <omp.h>

struct Nodo {
	std::vector<float> vDistancia;
	std::vector<unsigned int> vVecino;

	omp_lock_t lock_var;

	Nodo(unsigned int maxNN);

	unsigned int blind_insert(const float ds, const unsigned int idx);
	unsigned int safe_insert(const float ds, const unsigned int idx);
	void append(const float ds, const unsigned int idx);

	void sort();

	void lock();
	bool test_lock();
	void unlock();
};

#endif

