#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <tuple>
#include "treap.h"

//decode function
void decode(){

	srand((unsigned)time(NULL));

	int operation, index, item;

	//keep the input
	std::vector<std::tuple<int, int, int>> tupleVector;

	//read the input
	while(std::cin >> operation){
		std::cin >> index;
		if(operation == 4 || operation == 5){
				tupleVector.push_back(std::make_tuple(operation, index, NULL));
		}
		else{
			std::cin >> item;
			tupleVector.push_back(std::make_tuple(operation, index, item));
		}
	}


	long unsigned int n = tupleVector.size();
	std::vector<Treap<int> *> treapVector;
	Treap<int> * treap = new Treap<int>();
	
	treapVector.push_back(treap);
	
	bool found;

	//iterate through every operation from the input
	for(long unsigned int i = 0; i < n; ++i){
		operation = std::get<0>(tupleVector[i]);
		index = std::get<1>(tupleVector[i]);
		item = std::get<2>(tupleVector[i]);

		//std::cout << "==================" << '\n';
		switch(operation){
			case 1:
				treapVector.push_back(treapVector[index]->insert(item));
				break;
			case 2:	
				treapVector.push_back(treapVector[index]->remove(item));
				break;
			case 3:
				found = treapVector[index]->search(item);
				if(found){
					std::cout  << 1 << '\n';
				}
				else{
					std::cout << 0 << '\n';
				}
				break;
			case 4:
				std::cout << treapVector[index]->getMin() << '\n';
				break;
			case 5:
				treapVector[index]->print();
				break;			 
		}
	}

}
	
//main function
int main(){
	decode();
	return 0;
}