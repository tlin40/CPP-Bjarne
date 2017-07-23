#include "GUI.h"

//------------------------------------------------------ Lines_window
Lines_window::Lines_window(Point xy, int ww, int hh, const string& title)
    :Window{xy, ww, hh, title}, 
     next_button{Point{x_max()-150,0}, 70, 20, "Next point", cb_next/*[](Address, Address pw){ reference_to<Lines_window>(pw).next(); }*/},
     quit_button{Point{x_max()-70,0}, 70, 20, "Quit", cb_quit /*[](Address, Address pw){ reference_to<Lines_window>(pw).quit(); }*/}, 
     next_x{Point{x_max()-310,0}, 50, 20, "next x:"}, 
     next_y{Point{x_max()-210,0}, 50, 20, "next y:"}, 
     xy_out{Point{100,0}, 100, 20, "current (x,y):"},
     color_menu{Point(x_max()-70,30), 70, 20, Menu::vertical, "color"},
     menu_button{Point(x_max()-80,30), 80, 20, "color menu", cb_menu}{

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

//private
void Lines_window::quit(){
	hide();
}

void Lines_window::next(){

	int x = next_x.get_int();
	int y = next_y.get_int();

	lines.add(Point(x,y));

	// update current position 
	stringstream ss;
	ss << '(' << x << ',' << y << ')';
	xy_out.put(ss.str());

	redraw();

}

void Lines_window::cb_red(Address, Address pw){
	reference_to<Lines_window>(pw).red_pressed(); 
}

void Lines_window::cb_blue(Address, Address pw){
	reference_to<Lines_window>(pw).blue_pressed(); 
}

void Lines_window::cb_black(Address, Address pw){
	reference_to<Lines_window>(pw).black_pressed(); 
}

void Lines_window::cb_menu(Address, Address pw){
	reference_to<Lines_window>(pw).menu_pressed(); 
}

void Lines_window::cb_next(Address, Address pw){
	reference_to<Lines_window>(pw).next(); 
}

void Lines_window::cb_quit(Address, Address pw){
	reference_to<Lines_window>(pw).quit(); 
}