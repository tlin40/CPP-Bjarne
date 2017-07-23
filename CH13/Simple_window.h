#include "Window.h"

struct Simple_window: Graph_lib::Window{ // see E.3 for Window

	// constructor
	Simple_window(Point xy, int w, int h, const string& title);

	void wait_for_button(); // simple event loop

private:

	Button next_button;
	
	bool button_pushed;

	static void cb_next(Address, Address); // callback for next_button

	void next(); // action to be done when next_button is pressed

};

Simple_window::Simple_window(Point xy, int w, int h, const string& title)
	: Window(xy, w, h, title),
	  next_button(Point(x_max()-70,0),70,20,"Next",cb_next),
	  //next_button(Point(x_max()-70,0),70,20,"Next",[](Address, Address pw){reference_to<Simple_window>(pw).next();}),
	  button_pushed(false){
	  	
	  	attach(next_button);
	  
	  }

void Simple_window::cb_next(Address, Address pw){
	// call Simple_window::next() for the window located at pw
	reference_to<Simple_window>(pw).next();
}

void Simple_window::wait_for_button(){

	while(!button_pushed) Fl::wait();
	button_pushed = false;
	Fl::redraw();
}

void Simple_window::next(){
	button_pushed = true;
}