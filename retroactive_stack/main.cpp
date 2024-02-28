#include <iostream>
#include <tuple>
#include <vector>
#include "stack.h"
	
//decode function
void decode(){
	
	srand((unsigned)time(NULL));

	int operation, instant, item;

	//keep the input
	std::vector<std::tuple<int, int, int>> tupleVector;

	//read the input
	while(std::cin >> operation){
		std::cin >> instant;
		if(operation == 1 || operation == 6){
			std::cin >> item;
			tupleVector.push_back(std::make_tuple(operation, instant, item));
		}
		else{
			tupleVector.push_back(std::make_tuple(operation, instant, NULL));
		}
	}

	long unsigned int n = tupleVector.size();
	Stack<int, int> * stack = new Stack<int, int>();

	//iterate through every operation from the input
	for(long unsigned int i = 0; i < n; ++i){
		operation = std::get<0>(tupleVector[i]);
		instant = std::get<1>(tupleVector[i]);
		item = std::get<2>(tupleVector[i]);
		
		switch(operation){
			case 1:
				stack->insert(instant, "Push", item);
				break;
			case 2:	
				stack->insert(instant, "Pop");
				break;
			case 3:
				stack->remove(instant);
				break;
			case 4:
				std::cout << stack->size(instant) << '\n';
				break;
			case 5:
				std::cout << stack->top(instant) << '\n';
				break;
			case 6:
				std::cout << stack->k_th(instant, item) << '\n';
				break;
			case 7:
				stack->print(instant);
				break;			 
		}
	}
}

//main function
int main(){
	decode();
}