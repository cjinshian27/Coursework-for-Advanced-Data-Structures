#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>
#include <climits>
#include "dynammicSegmentTree.h"

//decode function
void decode(){

	unsigned int n;
	int start, end;
		
	//create a new segment tree
	DynammicSegmentTree * dynammicSegTree = new DynammicSegmentTree();
	
	int operation, value; 
	unsigned int id = 1;

	//read input
	while(std::cin >> operation){
		
		if(operation == 1){
			std::cin >> start >> end;
			dynammicSegTree->insert(start, end, id);
			++id;
			}	
		else if(operation == 2){
			std::cin >> value;
			dynammicSegTree->segments(value);
		}
		else{
			dynammicSegTree->print();
		}
	}
}	


//main function
int main(){
	decode();
}