#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include "deque.h"

/*********************************************************************************

	          						DECODING

**********************************************************************************/

void decode(){

	int operation, deque, item;
	
	//keep each line from the input
	std::tuple <int, int, int> operationTuple;
	
	//keep all input
	std::vector<std::tuple<int, int, int>> tupleVector;
	
	//read input from a file
	while(std::cin >> operation){
		std::cin >> deque;
		if(operation == 1 || operation == 2 || operation == 7){
			std::cin >> item;
			operationTuple = std::make_tuple(operation, deque, item);
			tupleVector.push_back(operationTuple);
		}	
		else{
			operationTuple = std::make_tuple(operation, deque, NULL);
			tupleVector.push_back(operationTuple);
		}
	}
	
	unsigned int n = tupleVector.size();

	Deque<int> * emptyDeque = new Deque<int>();
	
	std::vector<Deque<int> *> dequeVector;
	
	dequeVector.push_back(emptyDeque);

	//iterate through all commands from the input
	for(long unsigned int i = 0; i < n; ++i){
		
		operation = std::get<0>(tupleVector[i]);
		deque = std::get<1>(tupleVector[i]);
		item = std::get<2>(tupleVector[i]);

		switch(operation){	
			case 1:  
				dequeVector.push_back(dequeVector[deque]->pushFront(item)); 
				break;
			case 2: 
				dequeVector.push_back(dequeVector[deque]->pushBack(item)); 
				break;
			case 3: 
				dequeVector.push_back(dequeVector[deque]->popFront()); 
				break;
			case 4:	
				dequeVector.push_back(dequeVector[deque]->popBack()); 
				break;
			case 5:
				std::cout << dequeVector[deque]->front() << '\n'; 
				break;
			case 6:
				std::cout << dequeVector[deque]->back() << '\n'; 
				break;
			case 7:
				std::cout << dequeVector[deque]->k_th(item) << '\n'; 
				break;
			case 8:
				dequeVector[deque]->print();
				break;
		}
	}

	//delete every pointer to avoid memory leak
	for(long unsigned int i = 0; i < dequeVector.size(); i++){
		delete dequeVector[i];
	}

}

/*********************************************************************************

	          						MAIN FUNCTION

**********************************************************************************/
int main(){
	decode();
}	