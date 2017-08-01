#include "std_lib_facilities.h"

template<typename T> // prefix
class vvector{

	/* 
		invariant:
		1.	if 0<=n && n<sz, elem[n] is element n
		2.	sz<=space
		3.	sz<space there's space for (space-sz) doubles after elem[sz-1]
	*/

	int sz;
	T* elem;
	int space; // number of elements + "free space"/"slots"

public:

	// default constructor
	vvector()
		:sz{0}, elem{nullptr}, space{0}{}

	// constructor
	explicit vvector(int s) // allocates s doubles
		:sz{s}, elem{new T[s]}, space{s}{
			// initialization
			for(int i=0; i<s; ++i) elem[i] = 0; 
	
	}

	// constructor (initializer_list)
	vvector(initializer_list<T> lst)
		: sz(lst.size()), elem{new T[sz]}, space{sz}{
			// initialization using std::copy B.5.2
			copy(lst.begin(),lst.end(),elem); 
	
	}

	// copy constructor; initialization
	vvector(const vvector&);

	// copy assignment; reset, such as v2 = v1
	vvector& operator=(const vvector&);

	// move constructor
	vvector(vvector&&);

	// move assignment
	vvector& operator=(vvector&&);

	// destructor
	~vvector(){ 
		delete[] elem; // to free memory
	}

	//------------------------------------------------ functions

	// using [] notation
	T& operator[](int n){ // non-const version
		return elem[n];
	}

	T operator[](int n) const{ // const version
		return elem[n];
	};

	/* abandoned
	double get(int n){
		return elem[n];
	}

	void set(int n, double v){
		elem[n] = v;
	}
	*/

	int size() const { return sz; } // current size (elements are initialized)

	int capacity() const{ return space; } // current space (elements are not initialized)


	void reserve(int newalloc); // add space for new elements

	void resize(int newsize); // newsize (elements are initialized)
	
	void push_back(T d);

};


template<typename T> vvector<T>::vvector(const vvector& arg) 
	// allocate elements and initializa by std::copy
	:sz{arg.sz}, elem{new T[arg.sz]}, space{sz}{
		copy(arg.elem, arg.elem+arg.sz, elem); // B.5.2

}


template<typename T> vvector<T>& vvector<T>::operator=(const vvector& a){

	if(this==&a) return *this; // self-assignment, no work needed

	if(a.sz<=space){ // enough space, no need for new allocation

		for(int i=0; i<a.sz; ++i) elem[i] = a.elem[i];
		sz = a.sz;
		return *this;

	}

	// make this vector a copy of a
	T* p = new T[a.sz]; // allocate new space
	// for(int i=0; i<a.sz; ++i) p[i] = a.elem[i];
	copy(a.elem, a.elem+a.sz, p); // copy elements

	delete[] elem; // elem of this vector

	elem = p;
	space = sz = a.sz;
	return *this;

}

template<typename T> vvector<T>::vvector(vvector&& a)
	:sz{a.sz}, elem{a.elem}, space{sz}{
		
		a.space = a.sz = 0; // clear
		a.elem = nullptr; // clear

}

template<typename T> vvector<T>& vvector<T>::operator=(vvector&& a){ // move a to this vector

	delete[] elem; // clear
	
	elem = a.elem;
	space = sz = a.sz;

	a.elem = nullptr; // clear
	a.space = a.sz = 0; // clear 

	return *this;

}

