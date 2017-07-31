#include "vector.h"

void vvector::reserve(int newalloc){

	if(newalloc<=space) return; // never decrease allocation
	
	// allocate new space
	double *p = new double[newalloc];
	// copy and delete old elements
	for(int i=0; i<sz; ++i) p[i] = elem[i];
	delete[] elem;
	elem = p;
	space = newalloc;

}

void vvector::resize(int newsize){

	reserve(newsize);
	for(int i=sz; i<newsize; ++i) elem[i] = 0;
	sz = newsize;

}

void vvector::push_back(double d){

	if(space==0)
		reserve(8); // start with space for 8 elements
	else if(sz==space)
		reserve(2*space); // get more space

	elem[sz] = d; // add d at end
	++sz;

}