#ifndef POINT_GUARD
#define POINT_GUARD 1

#include "std_lib_facilities.h"

namespace Graph_lib{

	//------------------------------------------ Point
	struct Point{



		// coordinates
		int x;
		int y;

		// default constructor
		Point()
			:x{0}, y{0}{}

		// constructor
		Point(int xx, int yy)
			:x{xx}, y{yy}{}

	};

	//------------------------------------------ operators for Points
	bool operator==(Point a, Point b){
		return a.x==b.x && a.y==b.y;
	}

	bool operator!=(Point a, Point b){
		return !(a==b);
	}

}

#endif

