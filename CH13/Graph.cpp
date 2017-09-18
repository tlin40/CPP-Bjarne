#include "Graph.h"

namespace Graph_lib{

	//---------------------------------------------------- Shape
	 // public:
	void Shape::set_color(Color col){
		lcolor = col;
	}

	Color Shape::color() const{
		return lcolor;
	}

	void Shape::set_style(Line_style sty){
		ls = sty;
	}

	Line_style Shape::style() const{
		return ls;
	}

	void Shape::set_fill_color(Color col){
		fcolor = col;
	}

	Color Shape::fill_color() const{
		return fcolor;
	}

	Point Shape::point(int i) const{
		return points[i];
	}

	int Shape::number_of_points() const{
		return points.size();
	}

	void Shape::draw() const{

		// set lcolor and ls
		Fl_Color oldc = fl_color();
		fl_color(lcolor.as_int()); // set color
		fl_line_style(ls.style(),ls.width()); // set style

		// call Shape::draw_lines
		draw_lines();

		// back to default fl_color and fl_line_style
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

	//---------------------------------------------- Text
	void Text::draw_lines() const{

		fl_draw(lab.c_str(),point(0).x,point(0).y);

	}

	//---------------------------------------------- Lines
	void Lines::add(Point p1, Point p2){

		Shape::add(p1);
		Shape::add(p2);
	}

	void Lines::draw_lines() const{

		if(color().visibility()){ // Shape::color() && Color::visibility
			for(int i=1; i<number_of_points(); i+=2) // Shape::number_of_points
				fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y); // underlying line drawing function
		}

    }

	//---------------------------------------------- Open_polyline
	void Open_polyline::add(Point p){
		Shape::add(p);
	}

	void Open_polyline::draw_lines() const{

		if(color().visibility()){
			for(int i=1; i<number_of_points(); i+=1)
				fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y);
		}

	}

	//---------------------------------------------- Closed_polyline
	void Closed_polyline::draw_lines() const{

		Open_polyline::draw_lines();

		if(2<number_of_points() && color.visibility())
			fl_line(point(number_of_points()-1).x,
					point(number_of_points()-1).y,
					point(0).x,
					point(0).y);

	}

	//----------------------------------------------- Polygon
	void Polygon::add(Point p){

		int np = number_of_points();

		if(1<np){ // check if the new line parallel to the last one

			if(p==point(np-1)) error("polygon point equal to previous point");
			bool parallel;
			line_intersect(point(np-1),p,point(np-2),point(np-1),parallel);
			if(parallel) error("polygon points lie in a straight line");

		}

		for(int i=1, i<np-1, ++i){
			
			Point ignore(0,0); // point of intersection
			if(line_segment_intersect(point(np-1),p,point(i-1),point(i),ignore))
				error("intersect in polygon");

		}

		Closed_polyline::add(p);
	}

	void Polygon::draw_lines() const{

		if(number_of_points()<3) error("less than 3 points in a polygon"); 
		Closed_polyline::draw_lines();
	}

	//----------------------------------------------- Rectangle
	int Rectangle::height() const{
		return h;
	}

	int Rectangle::width() const{
		return w;
	}

	void Rectangle::draw_lines() const{

		if(fill_color().visibility()){

			fl_color(fill_color().as_int()); // set color
			fl_rectf(point(0).x,point(0).y,w,h);

		}

		if(color().visibility()){

			fl_color(color().as_int());
			fl_rect(point(0).x,point(0).y,w,h); // fl_rect for outlines

		}
		
	}

	//----------------------------------------------- Circle
	void Circle::set_radius(int rr){
		// set radius also reset top left corner
		set_point(0,Point{center().x-rr,center().y-rr}); // Shape::set_point
		r = rr;
	}

	Point Circle::center() const{
		return Point{point(0).x+r,point(0).y+r};
	}

	void Circle::draw_lines() const{

		if(color().visibility())
			fl_arc(point(0).x,point(0).y,r+r,r+r,0,360); // point(0): top left corner
		                                                  // r+r, r+r: width and height
		                                                  // 0, 360: beginnning and angle and end angle

	}

	//----------------------------------------------- Ellipse
	 // functions
	void Ellipse::set_major(int ww){

		set_point(0,Point{center().x-ww,center().y-h}); // upper left corner
		w = ww;

	}

	void Ellipse::set_minor(int hh){

		set_point(0,Point{center().x-w,center().y-hh});

	}

	Point Ellipse::center() const{

		return Point{point(0).x+w,point(0)+h};

	}

	Point Ellipse::focus1() const{

		if(h<=w) // foci are on the x axis
			return { center().x+sqrt(double{w*w-h*h}), center().y};
		else
			return { center().x, center().y+sqrt(double{h*h-w*w})};
	}

	Point Ellipse::focus2() const{

		if(h<=w)
			return { center().x-sqrt(double{w*w-h*h}) ,center().y };
		else
			return { center().x, center().y-sqrt(double{h*h-w*w})};

	}

	void Ellipse::draw_lines() const{

		if(color().visibility())
			fl_arc(point(0).x,point(0).y,w+w,h+h,0,360);

	}

	//------------------------------------------------ Marked_polyline
	void Marked_polyline::draw_lines() const{

		Open_polyline::draw_lines();
		for(int i=0; i<number_of_points(); ++i)
			draw_mark(point(i), mark[i%mark.size()]); // recall mark is a string

	}

	//------------------------------------------------ Image
	void Image::set_mask(Point xy, int ww, int hh){

		w = ww;
		h = hh;
		cx = xy.x;
		cy = xy.y;

	}

	void Image::draw_lines() const{

		if(fn.label()!="") fn.draw_lines();

		if(w&&h)
			p->draw(point(0).x,point(0).y,w,h,cx,cy); // Fl_Image* p
		else
			p->draw(point(0).x,point(0).y);

	}

	//----------------------------------------------- Axis
	void Axis::move(int dx, int dy){

		// line
		Shape::move(dx,dy);

		// notches and label
		notches.move(dx,dy);
		label.move(dx,dy);

	}

	void Axis::set_color(Color c){

		// line
		Shape::set_color(c);

		// notches and label
		notches.set_color(c);
		label.set_color(c);

	}

	void Axis::draw_lines() const{
		
		// line
		if(color().visibility()) // points.size() should be 2
			fl_line(points[1].x,point[0].y,points[1].x,points[1].y);

		// notches and label
		notches.draw();
		label.draw();

	}

}


