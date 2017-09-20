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
    void Button::attach(Window& w){ // called by Window::attach

        pw = new Fl_Button(loc.x,loc.y,width,height,label.c_str());
        pw->callback(reinterpret_cast<Fl_Callback*>(do_int),&w); // reinterpret the underlying bit pattern
        own = &w;

    }

    //------------------------------------------------- In_box
    int In_box::get_int(){

        Fl_Input& pi = reference_to<Fl_Input>(pw); // Fl_Input* to Fl_Input&
        const char* p = pi.value(); // current pointer to the text dispayed
        if(!isdigit(p[0])) return -999999;
        return atoi(p); // convert string to integer

    }

    string In_box::get_string(){

        Fl_Input& pi = reference_to<Fl_Input>(pw);
        return string(pi.value());

    }

    void In_box::attach(Window& w){

        pw = new Fl_Input(loc.x,loc.y,width,height,label.c_str());
        own = &w;

    }
    
    //------------------------------------------------- Out_box
    void Out_box::put(int i){

        Fl_Output& po = reference_to<Fl_Output>(pw);
        stringstream ss;
        ss << i;
        po.value(ss.str().c_str());

    }

    void Out_box::put(const string& s){

        reference_to<Fl_Output>(pw).value(s.c_str());

    }

    void Out_box::attach(Window& w){

        pw = new Fl_Output(loc.x,loc.y,width,height,label.c_str());
        own = &w;

    }

}



