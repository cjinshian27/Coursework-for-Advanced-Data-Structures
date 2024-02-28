#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <climits>
#include "certificate.h"
#include "node.h"
#include "minPriorityQueue.h"
#include "kineticHeap.h"

//decode function
void decode(){
	unsigned int n; std::cin >> n;
	double x0, velocity, time, xnow;
	KineticHeap * kineticHeap = new KineticHeap();
	int id = 1;
	for(unsigned int i = 0; i < n; ++i){
		std::cin >> x0 >> velocity;
		kineticHeap->insert(id++, x0, velocity);
	}	

	int operation;
	 while(std::cin >> operation){
		switch(operation){
			case 1:
				std::cin >> time;
				kineticHeap->advance(time);
				break;
			case 2:
				std::cin >> id >> velocity;
				kineticHeap->change(id, velocity);
				break;
			case 3:
				std::cin >> id >> xnow >> velocity;
				kineticHeap->insert(id, xnow - kineticHeap->currentTime() * velocity , velocity);
				break;
			case 4:
				std::cout << kineticHeap->max() << '\n';
				break;
			case 5:
				kineticHeap->remove(0);
				break;
			case 6:
				std::cin >> id;
				kineticHeap->removeNode(id);
				break;
			case 7:
				kineticHeap->printNodes();
				break;
		}
	}
}

//main function
int main(){

	decode();
}