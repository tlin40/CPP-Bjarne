struct Point{

	int x;
	int y;

	Point(int xx, int yy): x(xx), y(yy){}
	Point(): x(0), y(0){}
};

//------------------------------------------

bool operator==(Point a, Point b){
	return a.x==b.x && a.y==b.y;
}

bool operator!=(Point a, Point b){
	return !(a==b);
}