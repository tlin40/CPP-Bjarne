#include "Graph.h"

namespace Graph_lib{

	//---------------------------------------------------- helper functions
	pair<double,double> line_intersect(Point p1, Point p2, Point p3, Point p4, bool& parallel){ 
		double x1 = p1.x;
		double x2 = p2.x;
		double x3 = p3.x;
		double x4 = p4.x;
		double y1 = p1.y;
 		double y2 = p2.y;
		double y3 = p3.y;
		double y4 = p4.y;

		double denom = (y4-y3)*(x2-x1) - (x4-x3)*(y2-y1); // check if parallel
		if(denom == 0){
			parallel = true;
			return pair<double,double>(0,0); // return (0,0)
		}
		parallel = false;
		return pair<double,double>{((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3))/denom,
								   ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3))/denom};

	}

    bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4, Point& intersection){

	 	bool parallel;
	 	pair<double,double> u = line_intersect(p1,p2,p3,p4,parallel);

	 	if(parallel || u.first<0 || u.first>1 || u.second<0 || u.second>1) return false; // not parallel
	 	intersection.x = p1.x + u.first*(p2.x - p1.x);
	 	intersection.y = p1.y + u.first*(p2.y - p1.y);
	 	return true;

	 }

	void draw_mark(Point xy, char c){

		constexpr int dx = 4; // to center the letter over the point
		constexpr int dy = 4;

		string m{1,c}; // fill the string with one c

		fl_draw(m.c_str(),xy.x-dx,xy.y+dy);

	}

	bool can_open(const string& s){
		ifstream ff{s};
		return static_cast<bool>(ff); // here requires explicit conversion
	}

	map<string,Suffix::Encoding> suffix_map;

	int init_suffix_map(){

		suffix_map["jpg"] = Suffix::jpg;
		suffix_map["JPG"] = Suffix::jpg;
		suffix_map["jpeg"] = Suffix::jpg;
		suffix_map["JPEG"] = Suffix::jpg;
		
		suffix_map["gif"] = Suffix::gif;
		suffix_map["GIF"] = Suffix::gif;
		
		suffix_map["bmp"] = Suffix::bmp;
		suffix_map["BMP"] = Suffix::bmp;

		return 0; 

	}

	Suffix::Encoding get_encoding(const string& s){
		// try to deduce type
		static int x = init_suffix_map(); // initialize suffix_map once

		string::const_iterator p = find(s.begin(),s.end(),'.'); // to find '.' in the range [first,last)
		if(p==s.end()) return Suffix::none;

		string suf{p+1,s.end()}; // range constructor [first,last)
		return suffix_map[suf];

	}

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

	void Shape::draw_lines() const{
	} 

	// initializer_list constructor
	Shape::Shape(initializer_list<Point> lst){
		for(Point p: lst) add(p);
	}

	//---------------------------------------------- Text
	void Text::draw_lines() const{

		fl_draw(lab.c_str(),point(0).x,point(0).y);

	}

	//---------------------------------------------- Line
	Line::Line(Point p1, Point p2){
		add(p1);
		add(p2);
	}

	//---------------------------------------------- Lines
	Lines::Lines(initializer_list<pair<Point,Point>> lst){

		for(auto p: lst) add(p.first,p.second); // pair and initializer_list are defined in the std library

	}

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

		if(2<number_of_points() && color().visibility())
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

		for(int i=1; i<np-1; ++i){
			
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
	// constructors
	Rectangle::Rectangle(Point xy, int ww, int hh)
		:w{ww}, h{hh}{
			
			if(h<=0 || w<=0) error("Bad rectangle: non-positive side");
			add(xy);

	}

	Rectangle::Rectangle(Point x, Point y)
		: w{y.x-x.x}, h{y.y-x.y}{

		if(h<=0 || w<=0) error("Bad rectangle: non-positive width or height");
		add(x);

	}

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
	 // constructor
	Circle::Circle(Point p, int rr)
		:r{rr}{
			add(Point{p.x-r,p.y-r}); // add top left corner
	}

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
	// constructor
	Ellipse::Ellipse(Point p, int ww, int hh)
		:w{ww}, h{hh}{
			add(Point{p.x-w,p.y-h}); // add top left corner
	}

	 // functions
	void Ellipse::set_major(int ww){

		set_point(0,Point{center().x-ww,center().y-h}); // upper left corner
		w = ww;

	}

	void Ellipse::set_minor(int hh){

		set_point(0,Point{center().x-w,center().y-hh});

	}

	Point Ellipse::center() const{

		return Point{point(0).x+w,point(0).y+h};

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
	 // constructors
	Marked_polyline::Marked_polyline(const string& s)
		:mark{s}{
			if(mark=="") mark="*"; // default mark: *
	}

	Marked_polyline::Marked_polyline(const string& s, initializer_list<Point> lst)
		:Open_polyline{lst}, mark{s}{
			if(mark=="") mark="*";
	}

	void Marked_polyline::draw_lines() const{

		Open_polyline::draw_lines();
		for(int i=0; i<number_of_points(); ++i)
			draw_mark(point(i), mark[i%mark.size()]); // recall mark is a string

	}

	//------------------------------------------------ Marks
	 // constructors
	Marks::Marks(const string& s)
		: Marked_polyline{s}{
			set_color(Color{Color::invisible});
	}

	Marks::Marks(const string& s, initializer_list<Point> lst)
		: Marked_polyline{s,lst}{
			set_color(Color{Color::invisible});
	}

	//------------------------------------------------ Mark	
	 // constructor
	Mark::Mark(Point xy, char c)
		:Marks{string{1,c}}{
			add(xy);
	}

	//------------------------------------------------ Image
	 // constructor
	Image::Image(Point xy, string file_name, Suffix::Encoding e)
		: w{0}, h{0}, fn{xy,""}{

			add(xy); // origin/upperleft corner

			if(!can_open(file_name)){

				fn.set_label("cannot open \""+file_name+'"');
				p = new Bad_image(30,20); // struct Bad_image
				return;

			}
			if(e==Suffix::none) e = get_encoding(file_name); // if can_open and didn't specify e

			switch(e){

				case Suffix::jpg:
					p = new Fl_JPEG_Image{file_name.c_str()};
				break;

				case Suffix::gif:
					p = new Fl_GIF_Image{file_name.c_str()};
				break;

				default:
					fn.set_label("unsupported file style\""+file_name+'"');
					p = new Bad_image(30,20);

			}
	}

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
	 // constructor
	Axis::Axis(Orientation d, Point xy, int length, int number_of_notches, string lab)
		: label{Point(0,0),lab}{

			if(length<0) error("bad axis length");

			switch(d){

				case Axis::x://{

					add(xy);
					add(Point{xy.x+length,xy.y});

					if(0<number_of_notches){ // first notch at xy.x+dist; last notch at xy.x+length

						int dist = length/number_of_notches;
						int x = xy.x + dist;

						for(int i=0; i<number_of_notches; ++i){
							notches.add(Point{x,xy.y},Point{x,xy.y-5});
							x += dist;
						}

					}

					label.move(length/3,xy.y+20); // put the label under the line
					break;
				//}

				case Axis::y://{

					add(xy);
					add(Point{xy.x,xy.y-length});


					if(0<number_of_notches){

						int dist = length/number_of_notches;
						int y = xy.y - dist;

						for(int i=0; i<number_of_notches; ++i){
							notches.add(Point{xy.x,y},Point{xy.x+5,y});
							y -= dist;
						}
			
					}

					label.move(xy.x-10,xy.y-length-10); // put the label at the top
					break;
				//}

				case Axis::z:
					error("z axis not implemented");

			}

	}

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
			fl_line(point(1).x,point(0).y,point(1).x,point(1).y);

		// notches and label
		notches.draw();
		label.draw();

	}

	//---------------------------------------------------- Function
	 // constructor
	Function::Function(Fct f, double r1, double r2, Point orig, int count, double xscale, double yscale){
        // (0,0) at orig, plot the f from r1 to r2 relative to orig

		if((r2-r1)<=0) error("bad graphing range");
		if(count<=0) error("non-positive graphing count");

		double dist = (r2-r1)/count;
		double r = r1;
		for(int i=1; i<=count; ++i){
			add(Point{orig.x+int(r*xscale), orig.y-int(f(r)*yscale)});
			r += dist;
		}

	}

}


