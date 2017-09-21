#ifndef WINDOW_GUARD
#define WINDOW_GUARD 1

#include "GUI.h"

namespace Graph_lib{
	//--------------------------------------------------------------- Menu
	 // basically a vector of Buttons, a collection of Widgets
	struct Menu: Widget{

		enum Kind{ horizontal, vertical };

		// constructor
		Menu(Point xy, int ww, int hh, Kind kk, const string& ss)
			:Widget{xy,ww,hh,ss,0}, k{kk}, offset{0}{}

		// functions
		int attach(Button& b);
		
		int attach(Button* p);

		// virtual functions
		void move(int dx, int dy); // move all buttons

		void show(); // show all buttons

		void hide(); // hide all buttons

		void attach(Window& w); // attach all buttons to Window

		// data
		Kind k;
		int offset;
		Vector_ref<Button> selection;

	};

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
			int x_max() const;

			int y_max() const;
		
		protected:
			void draw();
		
		private:
			int w, h; // width and height
			vector<Shape*> shapes; // Shapes attached to Window
			void init();
	};

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

}

#endif
