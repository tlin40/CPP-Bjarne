#include "std_lib_facilities.h"

struct Link{

	string value;
	//Link* prev;
	//Link* succ;
	// constructor
	Link(const string& v, Link* p = nullptr, Link* s = nullptr)
		: value{v}, prev{p}, succ{s}{} 
		// nullptr means no prev or succ

	// functions
	Link* insert(Link* n); // insert n before this object
	Link* add(Link* n); // insert n after this object
	Link* erase(); // remove this object
	Link* find(const string& s); // find s in list
	Link* advance(int n); // move n positions
	Link* next() const{
		return succ;
	}
	Link* previous() const{
		return prev;
	}
	const Link* find(const string& s) const; // find s in const list

private:
	Link* prev;
	Link* succ;

};

//----------------------------------------------------------------- insert a prev

Link* Link::insert(Link* n){ // insert n before p; return n

	//Link* p = this; // pointer to this object

	if(n==nullptr) return this;
	if(this==nullptr) return n;

	n->succ = this; // p becomes succ of n
	if(prev){ // if p has prev
		prev->succ = n; 
	}
	n->prev = prev;
	prev = n;

	return n;

}

//---------------------------------------------------------------- add a succ

Link* Link::add(Link* n){

	if(n==nullptr) return this;
	if(this==nullptr) return n;

	n->prev = this;
	if(succ) succ->prev = n;
	n->succ = succ;
	succ = n;

	return n;
}

//---------------------------------------------------------------- erase a link

Link* Link::erase(){

	if(this==nullptr) return nullptr;
	if(succ) succ->prev = prev;
	if(prev) prev->succ = succ;
	return succ; // return next list

}

//---------------------------------------------------------------- find a link with a given value
																   // starting from p
Link* Link::find(const string& s){

	Link* next = this;

	while(next){

		if(next->value == s) {
			return next;
			}
		next = next->next();

	}

	return 0;

}

//---------------------------------------------------------------- move n position in lists
                                                                   // positive n moves forward, negative n moves backward
Link* Link::advance(int n){

	Link* next = this;

	if(next==nullptr) return nullptr;
	if(n>0){
		while(n--){
			if(next->next() == nullptr) return nullptr;
			next = next->next();
		}

	}
	if(0>n){
		while(n++){
			if(next->previous() == nullptr) return nullptr;
			next = next->previous();
		}

	}

	return next;

}

//----------------------------------------------------------------- print all

void print_all(Link* p){

	cout << "{ ";
	while(p){
		cout << p->value;
		if(p=p->next()) cout << ", "; // brilliant! first, check p->succ. second, store in p.
									// finally, p!=0 will do
	}
	cout << " }";
}

