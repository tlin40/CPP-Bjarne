#ifndef GRAPH_GUARD
#define GRAPH_GUARD 1

#include "fltk.h"
#include "Point.h"

namespace Graph_lib{

	// Vector_ref
	 // to handle new

	template<typename T> class Vector_ref{ // actually store addresses

		// private
		vector<T*> v;
		vector<T*> owned;

	public:

		// constructor
		Vector_ref(){}

		Vector_ref(T* a, T* b=0, T* c=0, T* d=0){

			if(a) push_back(a);
			if(b) push_back(b);
			if(c) push_back(c);
			if(d) push_back(d);

		}

		// destructor
		~Vector_ref(){
			for(int i=0; i<owned.size(); ++i){
				delete owned[i];
			}
		}

		// functions
		void push_back(T& s){ // add a named Shape
			v.push_back(&s);
		}
		
		void push_back(T* p){ // add an unnamed Shape
			v.push_back(p);
			owned.push_back(p);
		}

		T& operator[](int i){
			return *v[i];
		}

		T& operator[](int i) const{
			return *v[i];
		}

		int size() const{
			return v.size();
		}

	};

	typedef double Fct(double); // a typedef of function typpe maybe used to declare a function

	// helper functions
	 // does two lines (p1, p2) and (p3, p4) intersect?
	  // if so, return the distances of the intersect point from p1 to p2s and from p3 to p4
	pair<double,double> line_intersect(Point p1, Point p2, Point p3, Point p4, bool& parallel);
	  // from above, if intersect, returns true if the two segments intersect
	   // also return Point intersection 
	bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4, Point& intersection);

	 // draw_mark
	void draw_mark(Point xy, char c);

	// for Image
	struct Suffix{

		enum Encoding{ none, jpg, gif, bmp};
	
	};

	struct Bad_image: Fl_Image{

		// constructor
		Bad_image(int w, int h)
			:Fl_Image(w,h,0){}

		// function
		void draw(int x, int y, int, int, int, int){
			draw_empty(x,y);
		}

	};

	bool can_open(const string& s);

	int init_suffix_map();

	Suffix::Encoding get_encoding(const string& s);

	//------------------------------------------------------- Color
	struct Color{

		enum Color_type{

			red = FL_RED,
			blue = FL_BLUE,
			green = FL_GREEN,
			yellow = FL_YELLOW,
			white = FL_WHITE,
			black = FL_BLACK,
			magenta = FL_MAGENTA,
			cyan = FL_CYAN,
			dark_red = FL_DARK_RED,
			dark_green = FL_DARK_GREEN,
			dark_yellow = FL_DARK_YELLOW,
			dark_blue = FL_DARK_BLUE,
			dark_magenta = FL_DARK_MAGENTA,
			dark_cyan = FL_DARK_CYAN

		};

		enum Transparency{

			invisible = 0,
			visible = 255

		};

		// constructors
		Color(Color_type cc)
			: c{Fl_Color(cc)}, v(visible){} // first convert Color_type to Fl_Color

		Color(int cc)
			: c{Fl_Color(cc)}, v(visible){}

		Color(Transparency vv)
			: c{Fl_Color{}}, v(vv){}
		
		Color(Color_type cc, Transparency vv)
			: c{Fl_Color(cc)}, v(vv){}

		// functions
		void set_visibility(Transparency vv){
			v = vv;
		}

		int as_int() const {return c;}

		char visibility() const {return v;}

	private:
		char v;
		Fl_Color c; // Fl_Color type

	};

	//----------------------------------------------------- Line_style
	struct Line_style{

		enum Line_style_type{

			solid = FL_SOLID,
			dash = FL_DASH,
			dot = FL_DOT,
			dashdot = FL_DASHDOT,
			dashdotdot = FL_DASHDOTDOT

		};

		// constructors
		Line_style(Line_style_type ss)
			: s{ss}, w{0}{}
		Line_style(int ss)
			: s{ss}, w{0}{}
		Line_style(Line_style_type ss, int ww)
			: s{ss}, w{ww}{}

		// functions
		int width() const { return w; }

		int style() const { return s; }

	private:
	 	int s, w;
	};

	//----------------------------------------------------- Font
	class Font{

	public:
		enum Font_type{

			helvetica = FL_HELVETICA,
			helvetica_bold = FL_HELVETICA_BOLD,
			helvetica_italic = FL_HELVETICA_ITALIC,
			helvetica_bold_italic = FL_HELVETICA_ITALIC,
			courier = FL_COURIER,
			courier_bold = FL_COURIER_BOLD,
			courier_bold_italic = FL_COURIER_BOLD_ITALIC,
			symbol = FL_SYMBOL,
			screen = FL_SCREEN,
			screen_bold = FL_SCREEN_BOLD,
			sapf_dingbats = FL_ZAPF_DINGBATS

		};

		// consturctor
		Font(Font_type ff)
			: f{ff}{}
		
		Font(int ff)
			: f{ff}{}

		// function
		int as_int() const{
			return f;
		}
	
	private:
		int f;

	};

	//----------------------------------------------------- Shape
	class Shape{

	public:
		// set and read lcolor
		void set_color(Color col);
		Color color() const;

		// set and read style
		void set_style(Line_style sty);
		Line_style style() const;

		// set and read fcolor
		void set_fill_color(Color col);
		Color fill_color() const;

		// read points[i] and the number of points
		Point point(int i) const; // read points[i]
		int number_of_points() const;

        // set Line_style, Color, and call draw_lines
         // set fltk's fl_color and fl_line_style functions
		void draw() const;

		// virtual
		virtual void move(int dx, int dy);

		// prevent copying
		Shape(const Shape&) = delete; // delete copy constructor
		Shape& operator=(const Shape&) = delete; // delete copy assignment

		// pure virtual destructor
		virtual ~Shape(){} 

	protected:
		void add(Point p); // protected
		void set_point(int i, Point p); // set points[i]

		// virtual draw_lines()
		virtual void draw_lines() const; // draw lines (pixels on the screen)

		// constructor
		 // default
		Shape(){}
         // initializer_list constructor
		Shape(initializer_list<Point> lst);

	private:
		vector<Point> points;
		Color lcolor = fl_color(); // lcolor: for lines and texts
								  // fl_color: returns the last Fl_Color (unsigned int)
		Line_style ls{0};
		Color fcolor{Color::invisible}; // fcolor: for filling
	};

	//------------------------------------------------------- Text
	struct Text: Shape{

		// constructors
		Text(Point x, const string& s)
			:lab{s}{

				add(x);
		
		}

		// functions
		void set_label(const string& s){
			lab = s;
		}

		void set_font(Font f){
			fnt = f;
		}

		void set_font_size(int s){
			fnt_sz = s;
		}

		string label() const{
			return lab;
		}

		Font font() const{
			return fnt;
		}

		int font_size() const{
			return fnt_sz;
		}

		void draw_lines() const;

	private:
		string lab;
		Font fnt{fl_font()};
		int fnt_sz{fl_size()<14?14:fl_size()};

	};

	//----------------------------------------------------- Line
	 // Line is a Shape defined by two points
	struct Line: Shape{

		// constructor
		Line(Point p1, Point p2);

	};

	//----------------------------------------------------- Lines
	struct Lines: Shape{

		// constructor
		 // default
		Lines(){}
		 // initializer_list
		Lines(initializer_list<pair<Point,Point>> lst); // usage: Lines x = {
			                                 // {{100,100},{200,100}},
			                                 // {{150,50},{150,150}}
		                                     // };

		void add(Point p1, Point p2); // Lines::add

		void draw_lines() const;
	};

	//----------------------------------------------------- Open_polyline
	struct Open_polyline: Shape{

		// constructor
		 // default
		Open_polyline(){}
		 // initializer_list
		Open_polyline(initializer_list<Point> lst)
			: Shape{lst}{}

		void add(Point p); // because Shape::add is protected

		void draw_lines() const;

	};

	//----------------------------------------------------- Closed_polyline
	struct Closed_polyline: Open_polyline{

		using Open_polyline::Open_polyline; // lift Base's constructors into 
											 // Derived's scope (becoming Open_polyline's constructor,
											 // but won't initialize Derived's own members)

		void draw_lines() const;

	};

	//----------------------------------------------------- Polygon (Closed_polygon without line crossing)
	struct Polygon: Closed_polyline{

		using Closed_polyline::Closed_polyline;
		
		// functions
		void add(Point p); // overloading
		
		void draw_lines() const;

	};

	//----------------------------------------------------- Rectangle
	struct Rectangle: Shape{

		// constructors
		Rectangle(Point xy, int ww, int hh);

		Rectangle(Point x, Point y);

		// functions
		int height() const;

		int width() const;

		void draw_lines() const;

	private:
		int h, w;

	};

	//----------------------------------------------------- Circle
	struct Circle: Shape{

		// constructor
		Circle(Point p, int rr); // where p is center

		// functions
		void set_radius(int rr);

		int radius() const{
			return r;
		}

		Point center() const;

		void draw_lines() const;

	private:
		int r;

	};

	//----------------------------------------------------- Ellipse
	struct Ellipse: Shape{

		// constructor
		Ellipse(Point p, int ww, int hh); 

		// functions
		void set_major(int ww);

		void set_minor(int hh);

		int major() const{ return w; }

		int minor() const{ return h; }

		Point center() const;

		Point focus1() const;

		Point focus2() const;

		void draw_lines() const;

	private:
		int w, h; // w: major h: minor

	};

	//----------------------------------------------------- Marked_polyline
	struct Marked_polyline: Open_polyline{

		// constructor
		Marked_polyline(const string& s);

		Marked_polyline(const string& s, initializer_list<Point> lst);

		// function
		void draw_lines() const;

	private:
		string mark; // a string not a char

	};

	//----------------------------------------------------- Marks: marks without lines connecting to them
	struct Marks: Marked_polyline{

		// constructors
		Marks(const string& s);

		Marks(const string& s, initializer_list<Point> lst);

	};

	//----------------------------------------------------- Mark
	struct Mark: Marks{

		// constructor
		Mark(Point xy, char c);

	};

	//----------------------------------------------------- Image
	struct Image: Shape{

		// constructor
		Image(Point xy, string file_name, Suffix::Encoding e = Suffix::none);

		// destructor
		~Image(){
			delete p;
		}

		// funcitons
		void set_mask(Point xy, int ww, int hh);

		void draw_lines() const;

	private:
		int w, h; // define masking box within image with box's orgin at cx and cy which are local coordinates of the image
		int cx, cy; 
		Fl_Image* p;
		Text fn;

	};

	//----------------------------------------------------- Axis
	struct Axis: Shape{

		enum Orientation{ x, y, z};

		// constructor
		Axis(Orientation d, Point xy, int length, int number_of_notches=0, string lab="");

		// functions
		void move(int dx, int dy);

		void set_color(Color c); // overloading

		void draw_lines() const;

		// data
		Text label;
		Lines notches;

	};


	//----------------------------------------------------- Function
	struct Function: Shape{

		// constructor
		Function(Fct f, double r1, double r2, Point orig, int count=100, double xscale=25, double yscale=25);

	};

} // Graph_lib

#endif

