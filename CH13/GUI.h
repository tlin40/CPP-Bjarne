#include "Graph.h"

//-------------------------------------------------------------------- widget
class Widget{ // Widget is a handle to an Fl_widget
public:
	Widget(Point xt, int w, int h, const string& s, Callback cb);

	virtual void move(int dx, int dy); // moves the widget
	virtual void hide(); // makes the widget invisible
	virtual void show(); // makes the widget visible (default)
	virtual void attach(Window&) = 0; // pure virtual function

	Point loc;
	int width;
	int height;
	string label;
	Callback do_int;

protected:
	Window* own; // every widget belongs to a window
	Fl_Widget* pw; // connecton to the FLTK Widget

};

//-------------------------------------------------------------------- button
struct Button: Widget{

	Button(Point xy, int ww, int hh, const string& z, Callback cb)
		:Widget(xy, ww, hh, s, cb){}

	void attach(Window& win);
};

//-------------------------------------------------------------------- In_box

struct In_box: Widget{ // accept text typed into it

	In_box(Point xy, int ww, int hh, const string& s)
		: Widget(xy, ww, hh, s, 0){}

	int get_int();
	
	string get_string();

	void attach(Window& win);

};

//-------------------------------------------------------------------- Out_box

struct Out_box: Widget{ // present some message to a user

	Out_box(Point xy, int ww, int hh, const string& s)
		:Widget(xy, ww, hh, s, 0){}

	void put(int);

	void put(const string&); // ???

	void attach(Window& win);

};

//-------------------------------------------------------------------- Menu

struct Menu: Widget{ // basically a vector of Buttons

	Menu(Point xy, int ww, int hh, Kind kk, const string& s);

	enum Kind{ horizontal, vertical };

	// data
	Vector_ref<Button> selection;
	Kind k;
	int offset;
	int attach(Button& b); // attach Button to Menu
	int attach(Button* p); // attach new Button to Menu

	// virtual
	void show(){ // show all buttons

		for(Button& b: selection) b.show();

	}

	void hide(); // hide all buttons

	void move(int dx, int dy); // move all buttons

	void attach(Window& win); // attach all buttons to Window win

};

//--------------------------------------------------------------------- Lines_window

struct Lines_window: Window{

	Lines_window(Point xy, int ww, int hh, const string& title);

private:

	// data
	Open_polyline lines;

	// widgets
	Menu color_menu;
	Button menu_button;
	Button next_button; // add (next_x,next_y) to lines
	Button quit_button;
	In_box next_x;
	In_box next_y;
	Out_box xy_out;

	void change(Color c){
		lines.set_color(c);
	}

	void hide_menu(){

		color_menu.hide();
		menu_button.show();

	}

	// callbacks
	static void cb_red(Address, Address); // callback for red button
	static void cb_blue(Address, Address);
	static void cb_black(Address, Address);
	static void cb_menu(Address, Address);
	static void cb_next(Address, Address);
	static void cb_quit(Address, Address);

	// actions
	void red_pressed(){
		change(Color::red);
		hide_menu();
	}

	void blue_pressed(){
		change(Color::blue);
		hide_menu();
	}

	void black_pressed(){
		Change(Color::black);
		hide_menu();
	}

	void menu_pressed(){
		menu_button.hide();
		color_menu.show();
	}

	void next();

	void quit(); // to delete the Window

};

Lines_window::Lines_window(Point xy, int ww, int hh, const string& title){
	:Window{xy, ww, hh, title}, 
	 next_button{Point{x_max()-150,0}, 70, 20, "Next point", cb_next/*[](Address, Address pw){ reference_to<Lines_window>(pw).next(); }*/},
	 quit_button{Point{x_max()-70,0}, 70, 20, "Quit", cb_quit /*[](Address, Address pw){ reference_to<Lines_window>(pw).quit(); }*/}, 
	 next_x{Point{x_max()-310,0}, 50, 20, "next x:"}, 
	 next_y{Point{x_max()-210,0}, 50, 20, "next y:"}, 
	 xy_out{Point{100,0}, 100, 20, "current (x,y):"},
	 color_menu{Point(x_max()-70,30), 70, 20, Menu::vertical, "color"},
	 menu_button{Point(x_max()-80,30), 80, 20, "color menu", "color"}{

	 	color_menu.attach(new Button(Point(0,0), 0, 0, "red", cb_red));
	 	color_menu.attach(new Button(Point(0,0), 0, 0, "blue", cb_blue));
	 	color_menu.attach(new Button(Point(0,0), 0, 0, "black", cb_black));

	 	attach(next_button);
	 	attach(quit_button);
	 	attach(next_x);
	 	attach(next_y);
	 	attach(xy_out);
	 	xy_out.put("no point");
	 	attach(lines);
	 	attach(menu_button);
	 	attach(color_menu);
	 	color_menu.hide();

	}

