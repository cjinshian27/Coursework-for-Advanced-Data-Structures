#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>
#include <climits>
#include "segmentTree.h"

//decode function
void decode(){

	unsigned int n;
	int start, end;
	std::cin >> n;
	std::vector<Node *> intervals(n);
	for(unsigned int i = 0; i < n; ++i){	
		std::cin >> start >> end;
		Node * node = new Node(start, end, true, true);
		node->id = i + 1;
		intervals[i] = node;
	}

	SegmentTree * segtree = new SegmentTree(intervals);
	segtree->build();
	
	int operation, value; 

	while(std::cin >> operation){
		
		if(operation == 2){
			segtree->print();
		}
		else{
			std::cin >> value;
			segtree->segments(value);
		}
	}
}	


//main function
int main(){
	decode();
}