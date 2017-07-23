#include "Graph.h"

//---------------------------------------------------- Shape

// public:
void Shape::set_color(Color col){ // set color
	lcolor = col;
}

Color Shape::color() const{ // read color
	return lcolor;
}

void Shape::set_style(Line_style sty){ // set style
	ls = sty;
}

Line_style style() const{ // read style
	return ls;
}

void set_fill_color(Color col){ // set fill_color
	fcolor = col;
}

Color fill_color() const{ // read fill_color
	return fcolor;
}

Point Shape::point(int i) const{
	return points[i];
}; 

int Shape::number_of_points() const{
	return points.size();
};

void Shape::draw() const{
	// set FLTK's fl_color and fl_line_style functions
	Fl_Color oldc = fl_color(); // returns the last Fl_Color
	fl_color(lcolor.as_int()); // set color
	fl_line_style(ls.style(),ls.width()); // set style
	// call Shape::draw_lines
	draw_lines();
	// back to before
	fl_color(oldc);
	fl_line_style(0);
}

void Shape::move(int dx, int dy){ // x+=dx y+=dy
	for(int i=0; i<points.size(); ++i){
		points[i].x+=dx;
		points[i].y+=dy;
	}

}

// protected:
void Shape::add(Point p){
	points.push_back(p);
}

void Shape::set_point(int i, Point p){
	points[i] = p;
}

//--------------------------------------------------- Lines

void Lines::draw_lines() const{  

	//if(color().visibility()) // property of Lines' Color object
		for(int i=1; i<number_of_points(); i+=2) // number_of_points is defined in Shape
			fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y); // line drawing funciton 
																	  // from the underlying library
}

//--------------------------------------------------- Distribution

istream& operator>>(istream& is, Distribution& d){
	char ch1 = 0;
	char ch2 = 0;
	char ch3 = 0;
	Distribution dd;

	if(is >> ch1 >> dd.year
		  >> ch2 >> dd.young >> dd.middle >> dd.old 
		  >> ch3){
					if(ch1!='(' || ch2!=':' || ch3!=')'){
						is.clear(ios_base::failbit);
						return is;
					}

	}else{
		return is;
	}

	d = dd;
	return is;

}
