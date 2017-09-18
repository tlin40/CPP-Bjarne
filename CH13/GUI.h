#ifndef GUI_GUARD
#define GUI_GUARD 1

#include "Graph.h"

namespace Graph_lib{

	class Window; // forward declare

	typedef void* Address; // using Address as void*; void* is a pointer doesn't know what it points to
	typedef void (*Callback)(Address, Address);
	// introduce a function pointer called Callback
	 // here the function is a void function and takes arguments as Address, Address
	 // example: Callback pf = &f; *pf(addr, addr);

	template<typename W> W& reference_to(Address addr){ // addr is a pointer
		return *static_cast<W*>(addr);
	}

	//--------------------------------------------------------------- Widget
	class Widget{ // Widget is a handle to Fl_Widget
		public:
			// constructor
			Widget(Point xy, int ww, int hh, const string& ss, Callback cb)
				:loc(xy), width(ww), height(hh), label(ss), do_int(cb){}

			// destructor
			virtual ~Widget(){}

			// functions
			virtual void move(int dx, int dy); // move the Widget
			
			virtual void hide(); // make the Widget invisible
			
			virtual void show(); // make the Widget visible (default)

			virtual void attach(Window&) = 0; // pure virtual function
			
			// data
			Point loc;
			int width;
			int height;
			string label;
			Callback do_int; // function pointer or function name
			                  // function name can be viewed as an address

		protected:
			Window* own; // every Widget belongs to a Window
			Fl_Widget* pw; // connection to Fl_Widget; assinged from derived class

	};


	//--------------------------------------------------------------- Button
	struct Button: Widget{

		// constructor
		Button(Point xy, int ww, int hh, const string& ss, Callback cb)
			:Widget(xy, ww, hh, ss, cb){}

		// function
		void attach(Window& w);

	};

	//--------------------------------------------------------------- In_box (user can type a string)
	struct In_box: Widget{

		In_box(Point xy, int ww, int hh, const string& ss)
			:Widget(xy, ww, hh, ss, 0){} // 0: nullptr

		// function
		int get_int();

		string get_string();

		void attach(Window& w);

	};

	//--------------------------------------------------------------- Out_box (program displays a string)

	struct Out_box: Widget{ // present some message to users

		// constructor
		Out_box(Point xy, int ww, int hh, const string& ss)
			:Widget(xy, ww, hh, ss, 0){}

		// function
		void put(int);

		void put(const string&);

		void attach(Window& w);

	};


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

}

#endif