#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "suffixArray.h"

//print the indices vector
void print(const std::vector<unsigned int> & indices){		
	
	//if there are no occurences at all
	if(indices.size() == 0){
		std::cout << "\n";
		return;
	}

	std::cout << indices[0];
	for(unsigned int i = 1; i < indices.size(); ++i){
		std::cout << " " << indices[i];
	}
	std::cout << '\n';
}
	
//decode's function
void decode(){

	unsigned int operation;

	std::string text; 
	std::string word;
	std::string fileName;
	std::stringstream strStream;

	std::ifstream readFile;

	SuffixArray * suffixArray; 
	
	while(std::cin >> operation){
		switch(operation){
			case 1:
    			std::getline(std::cin, text);
				text = text.substr(1, text.size());
				suffixArray = new SuffixArray(text);
				break;
			case 2:
				std::cin >> fileName;
				readFile.open(fileName);
				strStream << readFile.rdbuf();
				text = strStream.str();

				suffixArray = new SuffixArray(text);
				break;
			case 3:
				std::getline(std::cin, word);
				word = word.substr(1, word.size());
				suffixArray->search(word);
				break;
			case 4:
				std::getline(std::cin, word);
				word = word.substr(1, word.size());
				print(suffixArray->occurences(word));
				break;
			case 5:
				std::getline(std::cin, word);
				word = word.substr(1, word.size());
				std::cout << suffixArray->countOccurences(word) << '\n';
				break;
			case 6:
				suffixArray->print();
				break;	
		}
	}
	delete suffixArray;
	suffixArray = nullptr;

}


//main function
int main(){
	decode();
}