#include "std_lib_facilities.h"

struct my_out_of_range{}; // class used to report range access errors

template<typename T, typename A = allocator<T>> struct vvector_base{
	// deal with memory rather than typed object
	A alloc;
	T* elem;
	int sz;
	int space;

	vvector_base(const A& a, int n)
		: alloc{a}, elem{a.allocate(n)}, sz{n}, space(n){}

	~vvector_base(){
		alloc.deallocate(elem,space);
	}
};

template<typename T, typename A = allocator<T>> // prefix
class vvector: private vvector_base<T,A>{

	/*
	A alloc; // use allocate to handle memory for elements

		invariant:
		1.	if 0<=n && n<sz, elem[n] is element n
		2.	sz<=space
		3.	sz<space there's space for (space-sz) doubles after elem[sz-1]

	int sz;
	T* elem; // elem is a pointer of T
	int space; // number of elements + "free space"/"slots"
	*/

public:
	// default constructor
	vvector()
		:vvector::sz{0}, vvector::elem{nullptr}, vvector::space{0}{}

	// constructor
	explicit vvector(int s, T val = T()) // allocates s Ts
		:vvector::sz{s}, vvector::elem{vvector::alloc.allocate(s)}, vvector::space{s}{
			// initialization
			for(int i=0; i<s; ++i) vvector::alloc.construct(&vvector::elem[i],val); 
	}

	// constructor (initializer_list)
	vvector(initializer_list<T> lst)
		: vvector::sz(lst.size()), vvector::elem{vvector::alloc.allocate(vvector::sz)}, vvector::space{vvector::sz}{
			// initialization using std::copy B.5.2
			copy(lst.begin(),lst.end(),vvector::elem); 
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
		vvector::alloc.deallocate(vvector::elem, vvector::space); // to free memory
	}

	// using [] notation
	T& operator[](int n){ // non-const version / unchecked access
		return vvector::elem[n];
	}

	T operator[](int n) const{ // const version / unchecked access
		return vvector::elem[n];
	};

	T& at(int n); // checked access

	const T& at(int n) const; // checked access

	int size() const { return vvector::sz; } // current size (elements are initialized)

	int capacity() const{ return vvector::space; } // current space (elements are not initialized)

	void reserve(int newalloc); // add space for new elements

	void resize(int newsize, T val=T()); // newsize (elements are initialized)
	
	void push_back(const T& d);

};


template<typename T, typename A> vvector<T,A>::vvector(const vvector& arg) 
	// allocate elements and initialize by std::copy
	:vvector::sz{arg.sz}, vvector::elem{vvector::alloc.allocate(arg.sz)}, vvector::space{vvector::sz}{
		copy(arg.elem, arg.elem+arg.sz, vvector::elem); // B.5.2

}


template<typename T, typename A> vvector<T,A>& vvector<T,A>::operator=(const vvector& a){

	if(this==&a) return *this; // self-assignment, no work needed

	if(a.sz<=this->space){ // enough space, no need for new allocation

		for(int i=0; i<a.sz; ++i) this->elem[i] = a.elem[i];
		this->sz = a.sz;
		return *this;

	}

	// make this vector a copy of a
	T* p = this->alloc.allocate(a.sz); // allocate new space
	copy(a.elem, a.elem+a.sz, p); // copy elements

	this->alloc.deallocate(this->elem, this->space);

	this->elem = p;
	this->space = this->sz = a.sz;
	return *this;

}

template<typename T, typename A> vvector<T,A>::vvector(vvector&& a)
	:vvector::sz{a.sz}, vvector::elem{a.elem}, vvector::space{vvector::sz}{
		
		a.space = a.sz = 0; // clear
		a.elem = nullptr; // clear

}

template<typename T, typename A> vvector<T,A>& vvector<T,A>::operator=(vvector&& a){ // move a to this vector

	this->alloc.deallocate(this->elem, this->space);
	
	this->elem = a.elem;
	this->space = this->sz = a.sz;

	a.elem = nullptr; // clear
	a.space = a.sz = 0; // clear 

	return *this;

}

/* A simplified version of std allocator

	template<typename T> class allocator{

	public:
		T* allocate(int n); // allocate space for n objects of type T
		void deallocate(T* p, int n); // deallocate n objects of type T with pointer p

		void construct(T* p, const T& v); // construct a T with the value v with pointer p
		void destroy(T* p); // destroy the T with pointer p

	}
*/