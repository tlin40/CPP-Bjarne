#ifndef WINDOW_GUARD
#define WINDOW_GUARD 1

#include "fltk.h"
#include "std_lib_facilities.h"
#include "Point.h"

#include "GUI.h"
#include "Graph.h"

namespace Graph_lib{

	//--------------------------------------------------------------- Window
	class Window: public Fl_Window{
		public:
			// constructors
			Window(int ww, int hh, const string& title); // let the system pick the location
			
			Window(Point xy, int ww, int hh, const string& title); // top left corner in xy

			// virtual destructor
			virtual ~Window(){}

			// functions
			void resize(int ww, int hh){
				w = ww;
				h = hh;
				size(ww,hh); // from Fl_Window
			}

			void set_label(const string& s){
				label(s.c_str()); // from Fl_Window
			}

			void attach(Shape& s);

			void attach(Widget& w);

			void detach(Shape& s);

			void detach(Widget& w);

			void put_on_top(Shape& p); // put p on top of other Shapes

			// const functions
			int x_max() const{
				return w;
			}
			int y_max() const{
				return h;
			}
		
		protected:
			void draw();
		
		private:
			int w, h; // width and height
			vector<Shape*> shapes; // Shapes attached to Window
			void init();
	};

	// constructor
	Window::Window(int ww, int hh, const string& title)
		:Fl_Window(ww,hh,title.c_str()),w(ww),h(hh){
			init();
	}

	Window::Window(Point xy, int ww, int hh, const string& title)
		:Fl_Window(xy.x,xy.y,ww,hh,title.c_str()),w(ww),h(hh){
			init();
	}

	//-------------------------------------------------------------- helper functions
	int gui_main(){ // invoke GUI library's main event loop
		return Fl::run();
	}; 

	int x_max(){ // width of screen in pixels
		return Fl::w(); 
	}

	int y_max(){ // height of screen in pixels
		return Fl::h(); 
	}

	//------------------------------------------------------------- Lines_window
	struct Lines_window: Window{

		// constructor
		Lines_window(Point xy, int ww, int hh, const string& ss);

		private:

			// Shape
			Open_polyline lines;
			// Widgets
			Menu color_menu;
			Button menu_button;
			Button next_button; // add next_y,next_y to lines
			Button quit_button;
			In_box next_x;
			In_box next_y;
			Out_box xy_out;

			// functions
			void change(Color c){
				lines.set_color(c);
			}

			void hide_menu(){
				color_menu.hide();
				menu_button.show();
			}

			// callback functions
			static void cb_red(Address, Address); // callback function for red button
			static void cb_blue(Address, Address);
			static void cb_black(Address, Address);
			static void cb_menu(Address, Address);
			static void cb_next(Address, Address);
			static void cb_quit(Address, Address);

			// actions
			void red_pressed();
			void blue_pressed();
			void black_pressed();
			void menu_pressed();
			void next();
			void quit(); // to delet the Window

	};

	// constructor
	Lines_window::Lines_window(Point xy, int ww, int hh, const string& ss)
		:Window{xy, ww, hh, ss},
		 menu_button{Point{x_max()-80,30}, 80, 20, "Color menu", cb_menu},
		 next_button{Point{x_max()-150,0}, 70, 20, "Next point", cb_next},
		 quit_button{Point{x_max()-70,0}, 70, 20, "Quit", cb_quit},
		 next_x{Point{x_max()-310,0}, 50, 20, "next x:"},
		 next_y{Point{x_max()-210,0}, 50, 20, "next y:"},
		 xy_out{Point{100,0}, 100, 20, "current (x,y):"},
		 color_menu{Point(x_max()-70,30), 70, 20, Menu::vertical, "color"}{

		 	color_menu.attach(new Button(Point(0,0), 0, 0, "red", cb_red));
		 	color_menu.attach(new Button(Point(0,0), 0, 0, "blue", cb_blue));
		 	color_menu.attach(new Button(Point(0,0), 0, 0, "black", cb_black));
		 	attach(color_menu);
		 	attach(menu_button);
		 	color_menu.hide();

		 	attach(next_button);
		 	attach(quit_button);
		 	attach(next_x);
		 	attach(next_y);
		 	attach(xy_out);
		 	xy_out.put("no point");
		 	
		 	attach(lines);
	}

}

#endif
