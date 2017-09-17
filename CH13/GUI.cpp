#include "GUI.h"

namespace Graph_lib{

    //------------------------------------------------- Widget
    void Widget::move(int dx, int dy){

        pw->position(loc.x+=dx, loc.y+=dy);

    }
            
    void Widget::hide(){
        pw->hide();
    }
            
    void Widget::show(){
        pw->show();
    }
                        
    //------------------------------------------------- Button
    void Button::attach(Window& w){

        pw = new Fl_Button(loc.x,loc.y,witdth,height,label.c_str());
        pw->callback(reinterpret_cast<Fl_Callback*>(do_int),&w);
        own = &w;

    }

    //------------------------------------------------- In_box
    int In_box::get_int(){

    }

    string In_box::get_string(){

    }

    void In_box::attach(Window& w){

        pw = new Fl_Input(loc.x,loc.y,width,height,label.c_str());
        own = &w;

    }
    
/*
    int In_box::get_int()
{
    Fl_Input& pi = reference_to<Fl_Input>(pw);
//  return atoi(pi.value());
    const char* p = pi.value();
    if (!isdigit(p[0])) return -999999;
    return atoi(p);
}

string In_box::get_string()
{
    Fl_Input& pi = reference_to<Fl_Input>(pw);
    return string(pi.value());
}


*/
    //------------------------------------------------- Out_box
    void Out_box::put(int x){

    }

    void Out_box::put(const string& s){

    }

    void Out_box::attach(Window& w){

    }

    //------------------------------------------------ Menu

};

