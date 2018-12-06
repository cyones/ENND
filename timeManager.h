#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H
#include <vector>
#include <algorithm>

class timeManager {
private:
	std::vector< unsigned char > edge_added;
	std::vector< unsigned char > edge_checked;
	std::vector< unsigned char > last_change;
	unsigned char iteration;
	unsigned int N;
	unsigned int K;
	unsigned char iter_to_freeze;
public:
	timeManager(unsigned int N, unsigned int K, unsigned char iter_to_freeze);
	void iterate();
	void checked(unsigned int i, unsigned int n1);
	void added(unsigned int i, unsigned int n1);
	bool needs_procesing(unsigned int i);
	bool needs_procesing(unsigned int i, unsigned int n1, unsigned int j);
	bool needs_procesing(unsigned int i, unsigned int n1, unsigned int j, unsigned int n2);
};

#endif


