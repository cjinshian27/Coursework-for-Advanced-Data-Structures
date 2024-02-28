#include <iostream>
#include <utility>
#include <unordered_map>
#include <vector>
#include <string>
#include <iterator>
#include <set>
#include "dynamicForest.hpp"

//decode function
void decode(){

	int operation, n, u, v;

	DynamicForest<int> * dynamicForest = new DynamicForest<int>();

	while(std::cin >> operation){
		switch(operation){
			case 1:
				std::cin >> n;
				break;
			case 2:
				std::cin >> u >> v;
				if(dynamicForest->isConnected(u, v)){
					std::cout << "True" << '\n';
				}
				else{
					std::cout << "False" << '\n';
				}
				break;
			case 3:
				std::cin >> u >> v;
				dynamicForest->link(u, v);
				break;
			case 4:
				std::cin >> u >> v;
				dynamicForest->cut(u, v);
				break;
			case 5:
				dynamicForest->print();
				break;
		}
	}

}

//main function
int main(){
	decode();
}