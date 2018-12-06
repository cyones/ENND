#include "flagVector.h"

flagVector::flagVector(unsigned int size) : v(size, 0), n(0) {}

bool flagVector::is_marked(unsigned int i) {
	return v[i] > n;
}

void flagVector::mark(unsigned int i) {
	v[i] = n+1;
}

void flagVector::clear_all() {
	n++;
}

