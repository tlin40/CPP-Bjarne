#include "Link.h"

int main(){

	Link* norse_gods = new Link("Thor");
	norse_gods = norse_gods->insert(new Link("Odin"));
	norse_gods = norse_gods->insert(new Link("Zeus"));
	norse_gods = norse_gods->insert(new Link("Freia"));

	Link* greek_gods = new Link("Hera");
	greek_gods = greek_gods->insert(new Link("Athena"));
	greek_gods = greek_gods->insert(new Link("Mars"));
	greek_gods = greek_gods->insert(new Link("Poseidon"));

	// correct errors
	Link* p = greek_gods->find("Mars");
	if(p) p->value = "Ares";

	p = norse_gods->find("Zeus");
	if(p){
		if(p==norse_gods) norse_gods = p->next();
		p->erase();
		greek_gods = greek_gods->insert(p);
	}

	print_all(norse_gods);
	cout << "\n";

	print_all(greek_gods);
	cout << "\n";

}