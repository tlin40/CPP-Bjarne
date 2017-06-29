#include "std_lib_facilities.h"
/* 
    Simple Calculator 

    Revision History:
        June 28, 2017 by TH Lin
        June 29, 2017 by TH Lin

    This program implements a basic expression calculator.
    Input from cin; output to cout.
    The grammar for input is:

    Calculation:
        Statement
        Print
        Quit
        Calculation Statement

    Statement:
        Declaration
        Expression

    Declaration:
        "let" Name "=" Expression

    Print:
        ;

    Quit:
        q

    Expression:
        Term
        Expression + Term
        Expression - Term

    Term:
        Primary
        Term * Primary
        Term / Primary
        Term % Primary

    Primary:
        Number
        (Expression)
        - Primary
        + Primary

    Number:
        floating-point-literal

    Input comes from cin through the Token_stream called ts.
*/

//---------------------------------------------------------------------------- global constants

const char number = '8'; // t.kind == number means that t is a number token
const char quit = 'q'; // t.kind == quit means that t is a quit Token
const char print = ';'; // t.kind == print means that t is a print Token
const string prompt = "> ";
const string result = "= "; // used to indicate that what follows is a result
const char name = 'a'; // name token
const char let = 'L'; // declaration token
const char set = 's';
const string declkey = "let"; // declaration keyword
const string skey = "set"; // change variable value

//---------------------------------------------------------------------------- variables

class Variable{
public:
    string name;
    double value;
    Variable (string n, double v)
        : name(n), value(v) {}
};

vector<Variable> var_table; // a collection of variables

double get_value(string s){ // get the value of a variable
    for(int i = 0; i<var_table.size(); ++i){
        if(var_table[i].name == s) return var_table[i].value;
    }
    error("get: undefined variable ", s);
}

void set_value(string s, double d){ // modify the value of a variable
    for(int i=0; i<var_table.size(); ++i)
        if(var_table[i].name == s){
            var_table[i].value = d;
            return;
        }
    error("set: undefined variable ", s);
}

bool is_declared(string var){ // check if a variable already exists
    for(int i=0; i<var_table.size(); ++i){
        if(var_table[i].name == var) return true;
    }
    return false;
}

double define_name(string var, double val){ // add a variable
    if(is_declared(var)){
        error(var, " declared twice");
    } 
    var_table.push_back(Variable(var, val));
    return val;
}

//----------------------------------------------------------------------------- tokens


class Token{
public:
    char kind;
    double value;
    string name;
    Token(char ch)    // make a Token from a char
        :kind{ch}, value(0) { } // or kind(ch)  
    Token(char ch, double val)     // make a Token from a char and a double
        :kind{ch}, value{val} { }
    Token(char ch, string n)
        :kind{ch}, name{n} {}
};

class Token_stream{
public:
    Token_stream() // make a Token_stream that reads from cin
        :full(false), buffer(0) {}
    Token get();
    void putback(Token t);
    void ignore(char c); // discard characters up to and including a c
private: // implementation details
    bool full{false};
    Token buffer;
};

void Token_stream::putback(Token t){

    if(full) error("putback() into a full buffer");
    buffer = t;
    full = true;
}

Token Token_stream::get(){

    if(full){
        full = false;
        return buffer;
    }

    char ch;
    cin >> ch;

    switch(ch){

        case print: // all applicable cases b/c no break
        case quit:
        case '(': 
        case ')': 
        case '+': 
        case '-': 
        case '*': 
        case '/': 
        case '%':
        case '=':
            return Token(ch);

        case '.':
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':{

            cin.putback(ch); // put digit back into the input stream
            double val;
            cin >> val;
            return Token(number,val);
        }
        
        default:
        if(isalpha(ch)){ // if letter
            cin.putback(ch);
            string s;

            while(cin.get(ch) && (isalpha(ch)||isdigit(ch))){ // cin.get also reads a "blank"
                s += ch;
            }
            
            cin.putback(ch);

            if(s==declkey){
                return Token(let);
            }
            if(s==skey){
                return Token(set);
            }

            return Token(name,s);       
        }


        error("Bad token");
    }
}

void Token_stream::ignore(char c){

    if(full && c==buffer.kind){
        full = false;
        return;
    }

    full = false;

    char ch = 0;
    while(cin>>ch){
        if (ch==c) return;
    }

}

Token_stream ts;

void clean_up_mess(){
    ts.ignore(print);
}
//------------------------------------------------------------------------------ grammars

double expression();  // forward declare 

double primary()     // read and evaluate a Primary
{
    Token t = ts.get();

    switch (t.kind) {
    case '(':    // handle '(' expression ')'
        {    
            double d = expression();
            t = ts.get();
            if (t.kind != ')') error("')' expected");
            return d;
        }
    case number:            // we use '8' to represent a number
        return t.value;  // return the number's value
    case '+':
        return +primary();
    case '-':
        return -primary();
    case name:
        return get_value(t.name);
    default:
        error("primary expected");
    }
}

double term()
{
    double left = primary();
    Token t = ts.get();     // get the "next" token

    while(true) {
        switch (t.kind) {
        case '*':
            left *= primary();
            t = ts.get(); // get next * or /
            break;
        case '/':
            {    
                double d = primary();
                if (d == 0) error("divide by zero");
                left /= d; 
                t = ts.get(); // get next * or /
                break;
            }
        /*
        case '%':{
            double d = primary();
            if (d == 0) error("devide by zero");
            left = fmod(left,d);
            t = ts.get();
            break;
        }
        */
        // alternative method
        case '%':{
            int i1 = narrow_cast<int>(left);
            int i2 = narrow_cast<int>(primary());
            if(i2 == 0) error("%: devide by zero");
            left = i1 % i2;
            t =ts.get();
            break;
        }

        default:
            ts.putback(t); 
            return left;
        }
    }
}

double expression()
{
    double left = term();      // read and evaluate a Term

    Token t = ts.get();     // get the "next" token
    while(true) {
        switch(t.kind) {
        case '+':
            left += term();    // evaluate Term and add
            t = ts.get(); // get next + or -
            break;
        case '-':
            left -= term();    // evaluate Term and subtract
            t = ts.get(); // get next + or -
            break;
        default:
            ts.putback(t);
            return left;       // finally: no more + or -: return the answer
        }
    }
}

double declaration(){
    Token t = ts.get();
    if(t.kind!=name){
        error("name expected in declaration");
    }
    string var_name = t.name;

    Token t2 = ts.get();
    if(t2.kind!='=') error("= missing in declaration of ", var_name);

    double d = expression();
    define_name(var_name,d);
    return d;
}

double change_declaration(){
    Token t = ts.get();
    if(t.kind!=name){
        error("name expected in declaration");
    }
    string var_name = t.name;

    Token t2 = ts.get();
    if(t2.kind!='=') error("= missing in declaration of ", var_name);

    double d = expression();
    set_value(var_name,d);
    return d;
}

double statement(){

    Token t = ts.get();
    switch(t.kind){
        case let:
            return declaration();
        case set:
            return change_declaration();
        default:
            ts.putback(t);
            return expression();
    }
}

void calculate(){

    while (cin){ // while all previous operations on cin have succeeded
        try{
        
            cout << prompt;
            Token t = ts.get();
            while(t.kind == print){ // first discard all "prints"
                t = ts.get();
            }
            if(t.kind == quit){
                return;
            }
            ts.putback(t);
            cout << result << statement() << '\n';
        
        }catch(exception& e){
            cerr << e.what() << endl;
            clean_up_mess();
        }
    }
}

//------------------------------------------------------------------------------ main

int main()
try {

    define_name("pi",3.14159);
    define_name("e", 2.718);

    calculate();
    keep_window_open();
    return 0;
}
catch (exception& e) {
    cerr << "exception: " << e.what() << endl;
    keep_window_open("~~");
    return 1;
}
catch (...) {
    cerr << "exception \n";
    keep_window_open ("~~");
    return 2;
}
