#include "std_lib_facilities.h"
#include "Point.h"
#include "Graph.h"

int main(){

	Simple_window win1(Point(100,100),600,400,"two lines");

	Line horizontal(Point(100,100),Point(200,100));
	Line vertical(Point(150,50),Point(150,150));

	win1.attach(horizontal);
	win1.attach(vertical);

	win.wait_for_button();

	//Point p;

	//cout << p.x << '\t' << p.y;
}