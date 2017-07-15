//------------------------------------------------------------ Color

struct Color{

	enum Color_type {

		red = FL_RED,
		blue = FL_BLUE,
		green = FL_GREEN,
		yellow = FL_YELLOW,
		white = FL_WHITE,
		black = FL_BLACK,
		magenta = FL_MAGENTA,
		cyan = FL_CYAN,
		dark_rad = FL_DARK_RED,
		dark_green = FL_DRAK_GREEN,
		dark_yellow = FL_DARK_YELLOW,
		dark_blue = FL_DARK_BLUE,
		dark_magenta = FL_DARK_MAGENTA,
		dark_cyan = FL_DRAK_CYAN

	};

	enum Transparency {

		invisible = 0;
		visible = 255;

	};

	Color(Color_type cc): c(Fl_Color(cc)), v(visible){}
	Color(Color_type cc, Transparency vv): c(Fl_Color(cc)), v(vv){}
	Color(int cc): c(Fl_Color(cc)), v(visible){}
	Color(Transparency vv): c(Fl_Color()), v(vv){} // default color

	int as_int() const {return c;}
	char visibility() const {return v;}
	void set_visibility(Transparency vv){ v=vv; }


private:
	char v;
	Fl_Color c; // FLTK's Fl_Color type


}

//------------------------------------------------------------ Line_style

struct Line_style{

	enum Line_style_type{

		solid = FL_SOLID,
		dash = FL_DASH,
		dot = FL_DOT,
		dashdot = FL_DASHDOT,
		dashdotdot = FL_DASHDOTDOT
	};

	Line_style(Line_style_type ss): s(ss), w(0){}
	Line_style(Line_style_type lst): s(lst), w(ww){}
	Line_style(int ss): s(ss), w(0){}

	int width() const:{ return w;}
	int style() const:{ return s;} 

private:
	int s;
	int w;

};


//------------------------------------------------------------ Line

struct Line: Shape{ // a Line is a Shape defined by two points

	Line(Point p1, Point p2); // constructor

};

Line::Line(Point p1, Point p2){

	add(p1);
	add(p2);

}

//------------------------------------------------------------ Lines

struct Lines: Shape{

	Lines(){} // constructor: empty
	Lines(initializer_list<Point> lst); // constructor: taking an initializer_list of pairs of Points
									   // each pair defining a line
									   // usage:
									   //  Lines x = {
									   //          {{100,100},{200,100}},
									   //          {{150,50},{150,150}}
                                       //  };

	void draw_lines() const; // const: can be called by a const object
	void add(Point p1, Point p2);

};

void Lines::Lines(initializer_list<pair<Point,Point>> lst){
	for(auto p: lst) add(p.first,p.second); // auto is a placeholder for the type pair<Point,Point>
		                                    // first and second are the names of a pair's first and second members
											// initializer_list and pair are defined in the std library
}

void Lines:add(Point p1, Point p2){

	Shape::add(p1); // Shape's add not Lines' add
	Shape::add(p2); 

}

void Lines::draw_lines() const{  

	if(color().visibility()) // property of Lines' Color object
		for(int i=1; i<number_of_points(); i+=2) // number_of_points is defined in Shape
			fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y); // line drawing funciton 
																	  // from the underlying library

}

//----------------------------------------------------------- Open_polyline

struct Open_polyline: Shape{

	//using Shape::Shape; // wrong
	Open_polyline(){}
	Open_polyline(initializer_list<Point> lst): Shape{lst}{}
	void add(Point p){ Shape::add(p);}

};

//----------------------------------------------------------- Closed_polyline

struct Closed_polyline: Open_polyline{
	using Open_polyline::Open_polyline; // use Open_poliline's constructors

	void draw_lines() const;
};

void Closed_polyline::draw_lines() const{

	Open_polyline::draw_lines();
	if(2<number_of_points() && color.visibility())
		fl_line(point(number_of_points()-1).x,
				point(number_of_points()-1).y,
				point(0).x,
				point(0).y);
}

//----------------------------------------------------------- Polygon

struct Polygon: Closed_polyline{

	using Closed_polyline::Closed_polyline;

	void add(Point p);
	void draw_lines() const;

};

void Polygon::add(Point p){

	//...
	Closed_polyline::add(p);

}

//----------------------------------------------------------- Rectangle

struct Rectangle: Shape{

	Rectangle(Point xy, int ww, int hh);
	Rectangle(Point x, Point y);

	void draw_lines() const;

	int height() const {return h;};
	int width() const {return w;};

private:
	int h;
	int w;
};

Rectangle::Rectangle(Pont xy, int ww, int hh): w(ww), h(hh){

	if(h<=0||w<=0) error("Bad rectangle: non-positive side");
	add(xy);

}

Rectangle::Rectangle(Point x, Point y): w(y.x-x.x), h(y.y-x.y){

	if(h<=0||w<=0) error("Bad rectangle: non-positive width or height");
	add(x);

}

Rectangle::draw_lines() const{

	if(fill_color().visibility()){
		fl_Color(fill_color().as_int());
		fl_rectf(point(0).x,point(0).y,w,h);
	}

	if(color().visibility()){
		fl_color(color().as_int());
		fl_rect(point(0).x,point(0).y,w,h); // fl_rect for outlines
	}

}

//----------------------------------------------------------- template

template<class T> class Vector_ref{

	public:
		//...
		void push_back(T&); // add a named object
		void push_back(T*); // add an unnamed object

		T& operator[](int i); // subscripting: read and write access
		const T& operator[](int i) const;

		int size() const;

};

//----------------------------------------------------------- Font

class Font{

public:
	enum Font_type{
		helvetica = FL_HELVETICA,
		helvetica_bold = FL_HELVETICA_BOLD,
		helvetica_italic = FL_HELVETICA_ITALIC,
		helvetica_bold_italic = FL_HELVETICA_BOLD_ITALIC,
		courier = FL_COURIER,
		courier_bold = FL_COURIER_BOLD,
		courier_bold_italic = FL_COURIER_BOLD_ITALIC,
		times = FL_TIMES,
		times_bold = FL_TIMES_BOLD,
		times_italic = FL_TIMES_ITALIC,
		times_bold_italic = FL_TIMES_BOLD_ITALIC,
		symbol = FL_SYMBOL,
		screen = FL_SCREEN,
		screen_bold = FL_SCREEN_BOLD,
		zapf_dingbats = FL_ZAPF_DINGBATS
	};

	Font(Font_type ff)
		: f(ff){}
	Font(int ff)
		: f(ff){}
	
	int as_int() const{ return f;}

private:
	int f;
};

//----------------------------------------------------------- Text

	struct Text: Shape{

		Text(Point x, const string& s)
			: lab(s), fnt(fl_font()), fnt_sz(fl_size()){
				add(x);
			}

		void draw_lines() const;
		void set_label(const string& s){ lab=s; }  
		string label() const{ return lab; }
		void set_font(Font f){ fnt=f; }
		Font font() const{ return fnt; }
		void set_font_size(int s){ fnt_sz=s; }
		int font_size() const{ return fnt_sz; }

	private:
		string lab;
		Font fnt{fl_font()};
		int fnt_sz{ (fl_size()<14)?14:fl_size() };

	};

	void Text::draw_lines() const{

		fl_draw(lab.c_str(),point(0).x,point(0).y);

	}

//----------------------------------------------------------- Circle

	struct Circle: Shape{

		Circle(Point p, int rr);

		void draw_lines() const;
		Point center() const;
		int radius() const {return r;}
		void set_radius(int rr){

			set_point(0,Point{center().x-rr,center().y-rr});
			r = rr;

		}

	private:
		int r;

	}

	Circle::Circle(Point p, int rr)
		:r(rr){
			add(Point(p.x-r,p.y-r));
		}

	Point Circle::center const{
		return Point(point(0).x+r,point(0).y+r);
	}

	void Circle::draw_lines() const{
		if(color().visibility())
			fl_arc(point(0).x,point(0).y,r+r,r+r,0,360); // point(0).x, point(0).y: top left corner
														 // r+r, r+r: width and height
														 // 0, 360: beginnning angle and end angle
	}
//----------------------------------------------------------- Ellipse
	struct Ellipse: Shape{

		Ellipse(Point p, int w, int h); // centerm max, and min distance from center

		void draw_lines() const;

		Point center() const;
		Point focus1() const;
		Point focus2() const;

		void set_major(int ww){

			set_point(0,Point{center().x-ww,center().y-h});
			w = ww;

		}

		int major() const{ return w; }

		void set_minor(int hh){

			set_point(0,Point{center().x-w,center().y-hh});
			h = hh;

		}

		int minor() const{ return h; }

	private:
		int w;
		int h;

	}

	Point focus1() const{

		if(h<=w) // foci are on the x axis
			return {center().x+int(sqrt(double(w*w-h*h))),center().y};
		else     // foci are on the y axis
			return {center().x,center().y+int(sqrt(double(h*h-w*w)))};



	}
//----------------------------------------------------------- draw_mark
	void draw_mark(Point xy, char c){

		constexpr int dx = 4; // to center the letter over the point
		constexpr int dy = 4;

		string m{1,c}; // string hoding the single char c

		fl_draw(m.c_Str(),xy.x-dx,xy.y+dy);
	}


//----------------------------------------------------------- Marked_polyline
	struct Marked_polyline: Open_polyline{

		Marked_polyline(const string& m)
			: mark(m){
				if(m=="") mark="*"; // to avoid draw_lines() accesses a character that isn't there
			}
		Marked_polyline(const string& m, initializer_list<Point> lst);

		void draw_lines() const;

	private:
		string mark;
	};

	void Marked_polyline::draw_lines() const{

		Open_polyline::draw_lines();
		for(int i=0; i<number_of_points(); ++i)
			draw_mark(point(i), mark[i%mark.size()]);

	}

	Marked_polyline::Marked_polyline(const string& m, initializer_list<Point> lst)
		:Open_polyline{lst}, mark{m}{
			if(m=="") mark="*";
		}

//----------------------------------------------------------- Marks: marks without lines connecting to them

	struct Marks: Marked_polyline{

		Marks(const string& m)
			: Marked_polyline{m}{
				set_color(Color{Color::invisible});
			}

		Marked_polyline(const string& m, initializer_list<Point> lst){
			: Marked_polyline{m,lst}{
				set_color(Color{Color::invisible});
			}	

	};

//----------------------------------------------------------- Mark

	struct Mark: Marks{
		Mark(Point xy, char c)
			: Marks(string(1,c)){
				add(xy);
			}

	};

//----------------------------------------------------------- Images

	enum class Suffix{
		none, jpg, gif
	};

	struct Bad_image: Fl_Image{
		
		Bad_image(int h, int w): Fl_Image(h,w,0){}
		
		void draw(int x, int y, int, int, int, int){
			draw_empty(x,y);
		}

	};

	struct Image: Shape{

		Image(Point xy, string file_name, Suffix::Encoding e = Suffix::none);
		~Image(){ delete p;}

		void draw_lines() const;
		voide set_mask(Point xy, int ww, int hh){
			w = ww; 
			h = hh; 
			cx = xy.x; 
			cy = xy.y;
		}

	private:
		int w, h; // define masking box within image relative to position (cx,cy)
		int cx,cy;
		Fl_Image* p;
		Text fn;

	};

	Image::Image(Point xy, string s, Suffix e)
		: w{0}, h{0}, fn{xy,""}{
			
		}