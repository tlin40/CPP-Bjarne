#include "std_lib_facilities.h"

class Punct_stream{

public:

	Punct_stream(istream& is) 
		: source(is), sensitive(true){}

	void whitespace(const string& s){ 
		white = s;
	}

	void add_white(char c){ 
		white += c;
	}

	bool is_whitespace(char c);

	void case_sensitive(bool b){
		sensitive = b;
	}

	bool is_case_sensitive(){
		return sensitive;
	}

	Punct_stream& operator>>(string& s);

	operator bool();

private:
	istream& source;
	istringstream buffer;
	string white;
	bool sensitive;

};

Punct_stream::operator bool(){
	/*
		the result of ps>>s is a Punct_stream
		so we need a way to implicitly turn a Punct_stream into a bool
		in particular, it returns true if the operation on the Punct_stream succeeded
	*/

	/*
	if(buffer.bad()){
		cout << "bad\n";

	}else if(buffer.fail()){
		cout << "fail\n";

	}else if(buffer.eof()){
		cout << "eof\n";

	}else if(buffer.good()){
		cout << "good\n";
	}
	*/

	return !(buffer.fail()||buffer.bad()) && source.good(); // if buffer.fail() or buffer.bad() then returns false

}

bool Punct_stream::is_whitespace(char c){

	for (int i=0; i<white.size(); ++i)
		if(c==white[i]) return true;
	
	return false;

}

Punct_stream& Punct_stream::operator>>(string& s){

	static int count = 0;

	// a loop
	while(!(buffer>>s) && count==0){ // if eof() and count 0

		// buffer>>s returns eof for last word
		// buffer>>s returns fail if no words

		/*
		if(buffer.bad()){
			cout << "bad\n";

		}else if(buffer.fail()){
			cout << "fail\n";

		}else if(buffer.eof()){
			cout << "eof\n";

		}else if(buffer.good()){
			cout << "good\n";
		}
		*/

		if(buffer.bad()||!source.good()) return *this;
		
		buffer.clear(); // if source.good() and !buffer.bad()

		string line;
		getline(source,line);

		for(char& ch: line){
			if(is_whitespace(ch)){
				ch = ' ';
			}
			else if(!sensitive)
				ch = tolower(ch);
		}

		count += 1;
		buffer.str(line);

	}

	return *this;

}

int main(){
	/*
		given text input, produce a sorted list of all words in that text
		ignore punctuation and case differences
		eliminate duplicates from the output
	*/

	Punct_stream ps{cin};
	ps.whitespace(";:,.?!()\"{}<>/&$@#%^*|~");
	ps.case_sensitive(false); // don't distinguish upper or lower case

	cout << "Please enter words\n";
	vector<string> vs;
	for(string word; ps>>word; ){ // call >> repeatly
		//cout << word << "\n";
		vs.push_back(word);
	}

	sort(vs.begin(),vs.end());
	
	for(int i=0; i<vs.size(); ++i)
		if(i==0 || vs[i]!=vs[i-1]) cout << vs[i] << '\n';


}