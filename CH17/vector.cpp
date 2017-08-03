#include "vector.h"

template<typename T, typename A> T& vvector<T,A>::at(int n){
	
	if(n<0 || this->sz<=n) throw my_out_of_range();
	return this->elem[n];

}

template<typename T, typename A> const T& vvector<T,A>::at(int n) const{
	
	if(n<0 || this->sz<=n) throw my_out_of_range();
	return this->elem[n];

}

/*
template<typename T, typename A> void vvector<T,A>::reserve(int newalloc){

	if(newalloc<=this->space) return; // never decrease allocation
	
	vvector_base<T,A> b(this->alloc,newalloc); // allocate new space
	uninitialized_copy(this->elem,this->elem+this->sz,b.elem); // copy: uninitialized_copy can handle the throw

	for(int i=0; i<this->sz; ++i)
		this->alloc.destroy(&this->elem[i]); 
	this->alloc.deallocate(this->elem,this->space);// deallocate old space
		
	swap<vvector_base<T,A>>(*this,b); // swap representations

}
*/

template<typename T, typename A> void vvector<T,A>::resize(int newsize, T val){
	// that is, use T() as the default value unless the user says otherwise

	reserve(newsize);

	for(int i=this->sz; i<newsize; ++i)
		this->alloc.construct(&this->elem[i],val);

	for(int i = newsize; i<this->sz; ++i)
		this->alloc.destroy(&this->elem[i]);

	this->sz = newsize;

}

template<typename T, typename A> void vvector<T,A>::push_back(const T& val){

	if(this->space==0)
		reserve(8); // start with space for 8 elements
	else if(this->sz==this->space)
		reserve(2*this->space); // get more space

	this->alloc.construct(&this->elem[this->sz],val);
	++this->sz;

}

//--------------------------------------------------- reserve (unique_ptr)
template<typename T, typename A> void vvector<T,A>::reserve(int newalloc){
	
	if(newalloc<=this->space) return;

	unique_ptr<vvector_base<T,A>> p{new vvector_base<T,A>(this->alloc,newalloc)};
	uninitialized_copy(this->elem,this->elem+this->sz,p->elem);

	swap<vvector_base<T,A>>(*this,*p);

}

//---------------------------------------------------- helper function

void print_some(vvector<int>& v){

	int i = -1;
	cin >> i;
	while(i!=-1) try{
		cout << "v[" << i << "]==" << v.at(i) << "\n";
	}catch(my_out_of_range){
		cout << "bad index: " << i << "\n";
	}

}

int main(){

}
