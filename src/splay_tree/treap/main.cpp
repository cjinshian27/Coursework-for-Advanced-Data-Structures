#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "treap.h"

//decode function
void decode(){

	srand((unsigned)time(NULL));
	int operation, key;
	
	Treap<int> * treap = new Treap<int>();

	//iterate through every operation from the input
	while(std::cin >> operation){
		switch(operation){
			case 1:
				std::cin >> key;
				treap->insert(key);
				break;
			case 2:
				std::cin >> key;
				treap->remove(key);
				break;
			case 3:
				std::cin >> key;
				if(treap->search(key)){
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
	delete treap;
	treap = nullptr;
}
	
//main function
int main(){
	decode();
	return 0;
}