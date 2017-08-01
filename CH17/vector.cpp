#include "vector.h"

template<typename T> void vvector<T>::reserve(int newalloc){

	if(newalloc<=space) return; // never decrease allocation
	
	// allocate new space
	T* p = new T[newalloc];
	// copy and delete old elements
	for(int i=0; i<sz; ++i) p[i] = elem[i];
	delete[] elem;
	elem = p;
	space = newalloc;

}

template<typename T> void vvector<T>::resize(int newsize){

	reserve(newsize);
	for(int i=sz; i<newsize; ++i) elem[i] = 0;
	sz = newsize;

}

template<typename T> void vvector<T>::push_back(T d){

	if(space==0)
		reserve(8); // start with space for 8 elements
	else if(sz==space)
		reserve(2*space); // get more space

	elem[sz] = d; // add d at end
	++sz;

}

vector<Shape> vs;
vector<Circle> vc;
vs = vc; // error: vector<Shape> required

void f(vector<Shape>&);
f(vc); // error: vector<Shape> required

void f(vector<Shape&>){
	v.push_back(new Rectangle(Point(0,0),Point(100,100)));
	// how can we store Rectangle* inside vector<Circle*>?
}



