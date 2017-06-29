#include "std_lib_facilities.h"

int main(){

    // a vector stores 4 int from 0 to 9
	vector<int> v(4);
	vector<int> guess = {-1,-1,-1,-1};
	char e = 'a';
	int n = ' ';

	cout << "Let's play!\n";
	while(e!='e'){

		while(e=='a' || e=='b'){

			if(e!='b'){
				cout << "Choose a random question by entering an random int:";
				cin >> n;
			}

			e = ' ';
			for(int i=0; i<v.size(); ++i){
				srand(n+i);
				v[i] = rand()%10; 
				//cout << v[i] << ' ';
			}
		
			int r = 0;
		 	for(int i=1; i<v.size(); ++i){ // check if repeated
				for(int j=0; j<i; ++j){
					if(v[i]==v[j]){
						r += 1;
					}
				}
			}			
			if(r>0){
				e = 'b';
				cout << "Oops! choose another random question by entering an random int:";
				cin >> n;
			}
		}



		int temp = 0;
		cout << "Enter different 4 integers between 0 and 9: ";
		// check if numbers are good
		for(int i=0; i<guess.size(); ++i){
			if(cin>>temp){ // check if integer
				if(temp<0 || temp>9){ // check if 0 to 9
	 				error("not a legit number");
				}else{ 
					guess[i] = temp;
				}
			}else{
				error("not a integer\n");
			}
		}

		for(int i=1; i<guess.size(); ++i){ // check if repeated
			for(int j=0; j<i; ++j){
				if(guess[i]==guess[j]){
					error("some numbers are repeated\n");
				}
			}
		}
		
		

		int bulls = 0;
		int cows = 0;

		// comparison
		for(int i=0; i<guess.size(); ++i){
		    // bulls
			if(guess[i] == v[i]){
				bulls += 1;
			}else{ // cows
				for(int j=0; j<guess.size(); ++j){
					if(guess[i]==v[j] && i!=j){
						cows += 1;
					}
				}
		    }
		}

		if(bulls==4){
			cout << "You win! Press 'e' to exit or 'a' to start a new run:";
			cin >> e;
		}else{
			cout << "You have " << bulls << " bulls and " << cows << " cows. Guess again!\n";
		}
	}


}


