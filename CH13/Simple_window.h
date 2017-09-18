#ifndef SIMPLE_WINDOW_GUARD
#define SIMPLE_WINDOW_GUARD 1

#include "Window.h"

/*
	there's no Simple_window.cpp because of laziness :)
*/

namespace Graph_lib{

	struct Simple_window: Window{ // see Window.h

	// public:
		// constructor
		Simple_window(Point xy, int w, int h, const string& title)
			: Window{xy, w, h, title},
			  next_button{Point{x_max()-70,0},70,20,"Next",cb_next},
			  button_pushed{false}{

			  	attach(next_button);

		}

		void wait_for_button(){ // simple event loop

			while(!button_pushed) Fl::wait();
			button_pushed = false;
			Fl::redraw();

		}
	private:
		Button next_button;

		bool button_pushed;

		static void cb_next(Address, Address pw){ // callback for next_button

			reference_to<Simple_window>(pw).next();

		} 

		void next(){ // action to be done when next_button is pressed
			button_pushed = true;
		}
	};



}

#endif