#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "treap.h"

//decode function
void decode(){

	srand((unsigned)time(NULL));
	int operation, item;

	//keep the input
	std::vector<std::pair<int, int>> pairVector;
	
	//read the input
	while(std::cin >> operation){
		if(operation == 4 || operation == 5){
				pairVector.push_back({operation, NULL});
		}
		else{
			std::cin >> item;
			pairVector.push_back({operation, item});
		}
	}

	Treap<int> * treap = new Treap<int>();
	bool found;
	long unsigned int n = pairVector.size();
	
	//iterate through every operation from the input
	for(long unsigned int i = 0; i < n; ++i){
		
		operation = pairVector[i].first;
		item = pairVector[i].second;
		
		switch(operation){
			case 1:
				treap->insert(item);
				break;
			case 2:
				treap->remove(item);
				break;
			case 3:
				found = treap->search(item);
				if(found){
					std::cout  << 1 << '\n';
				}
				else{
					std::cout << 0 << '\n';
				}
				break;
			case 4:
				std::cout << treap->getMin() << '\n';
				break;
			case 5:
				treap->print();
				break;			 
		}
	}


}
	
//main function
int main(){
	decode();
	return 0;
}