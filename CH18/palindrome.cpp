#include "std_lib_facilities.h"

//--------------------------------------------------- string version
bool is_palindrome(const string& s){

	int first = 0;
	int last = s.length()-1;

	while(first<last){

		if(s[first]!=s[last]) return false;
		++first;
		--last;

	}
	return true;

}

//--------------------------------------------------- array version

bool is_palindrome(const char s[], int n){ // array of n chars

	int first = 0;
	int last = n-1;

	while(first<last){

		if(s[first]!=s[last]) return false;
		++first;
		--last;

	}
	return true;
 
}

istream& read_word(istream& is, char* buffer, int max){
	// read at most max-1 characters from is into buffer
	is.width(max); // reas at most max-1 characters in the next >>
	
	is >> buffer; // read whitespace-terminated word,
				  // add zero after the last character

	return is;

}

//--------------------------------------------------- pointer version

/*
bool is_palindrome(const char* first, const char* last){ // mutable pointer to an immutable object
	// first points to the first letter
	// last points to the last letter
	while(first<last){

		if(*first!=*last) return false;
		++first;
		--last;
	
	}

	return true;

}
*/

//--------------------------------------------------- fun version

bool is_palindrome(const char* first, const char* last){

	if(first<last){

		if(*first!=*last) return false;
		return is_palindrome(first+1,last-1);

	}
	return true;

}

int main(){

/* string
	for(string s; cin>>s;){
		cout << s << " is";
		if(!is_palindrome(s)) cout << " not";
		cout << " a palindrome\n";
	}
*/

/* array 
	constexpr int max = 128; // in fact 127 (1 for zero)
 	for(char s[max]; read_word(cin,s,max);){

		cout << s << " is";

		if(!is_palindrome(s,strlen(s))) cout << " not";
		cout << " a palindrome\n";

	}
*/

	const int max = 128;
	for(char s[max]; read_word(cin,s,max);){
		cout << s << " is";
		if(!is_palindrome(&s[0],&s[strlen(s)-1])) cout << " not";
		cout << " a palindrome\n";
	}


}