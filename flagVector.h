#ifndef FLAGVECTOR_H
#define FLAGVECTOR_H
#include <vector>

class flagVector {
private:
	std::vector<unsigned int> v;
	unsigned int n;
public:
	flagVector(unsigned int size);
	bool is_marked(unsigned int i);
	void mark(unsigned int i);
	void clear_all();
};

#endif
