#include "node.h"

class SuffixTree{
	private:	
		std::string text;
		Node * root;
		std::vector<unsigned int> * lcp;
		std::vector<std::pair<std::string, unsigned int>> * suffixVector;

		std::vector<unsigned int> lcpRemake;
		std::vector<unsigned int> suffixVectorRemake;
		unsigned int alphabetSize = 0;
		unsigned int leftInterval = 0;

		//build the lcp vector 
		void buildLCP(){
			unsigned int i = 0, j = 0, k;
			unsigned int n = text.size();
			std::vector<unsigned int> rank(n);
			while(i < n){
				rank[suffixVector->at(i).second] = i;
				++i;
			}
			i = 0; 
			while(i < n){
				if(rank[i] > 0){
					k = suffixVector->at(rank[i] - 1).second;
					while(text[i + j] == text[j + k]){
						++j;
					}
					lcp->at(rank[i]) = j;
					if(j > 0){
						--j;
					}
				}
				++i;
			}
		}

		//expand the node 
		void expandNode(Node * & node, Node * child){
			node->children->at(node->childrenNumber) = child;
			++node->childrenNumber;
			if(child != nullptr){
				child->parent = node;
			}
		}		

		//build the suffix and lcp vectors, as well as the suffix tree
		void buildVectors(){
			unsigned int n = text.size();

			suffixVector = new std::vector<std::pair<std::string, unsigned int>>(n);

			lcp = new std::vector<unsigned int>(n, 0);

			for(unsigned int i = 0; i < n; ++i){
				suffixVector->at(i) = {text.substr(i, n - i), i};
			}
			std::sort(suffixVector->begin(), suffixVector->end());
			
			alphabetSize = (unsigned int) suffixVector->at(n - 1).first[0];
			leftInterval = (unsigned int) suffixVector->at(1).first[0];

			buildLCP();
			buildCartesianTree();
		}

		//get all occurences indices of a given word in the text
		unsigned int getOccurences(std::vector<unsigned int> & positionsVector, 
			Node * & node, unsigned int index){
			if(node->isLeaf){
				positionsVector.push_back(node->suffixIndex);
				return 1;
			}
			else{
				unsigned int n = node->childrenNumber;
				unsigned int c = node->firstChild;
				unsigned int i = index;
				
				for(unsigned int k = 0; k < n; ++k){
					
					i = i + getOccurences(positionsVector, node->children->at(c), i);
					
					c = node->childrenPosition->at(c);
				}

				return (i - index);
			}
		}
		
		//print out the suffix tree recursively
		void print(Node * & node, unsigned int depth){
			if(node->isLeaf){
				for(unsigned int j = 0; j < depth; ++j){
					std::cout << ' ';
				}
				node->print();
				return;	
			}
			else{
				unsigned int n = node->childrenNumber;
				unsigned int c = node->firstChild;
	
				for(unsigned int i = 0; i < n; ++i){
					
					print(node->children->at(c), depth + 3);
					c = node->childrenPosition->at(c);

					if(i == n - 1){
						break;
					}
					for(unsigned int j = 0; j < depth; ++j){
							std::cout << ' ';
					}
					node->print();
				}
			}
		}

		//build suffix tree using stack 
		void buildCartesianTree(){
			if(text.size() == 1){
				root = nullptr;
				return;
			}
			Node * aux;
			Node * node = new Node(this->lcp->at(1), false, alphabetSize);
			expandNode(node, nullptr);

			std::stack<Node *> nodeStack;
			nodeStack.push(node);

			unsigned int n = text.size();
				

			for(unsigned int i = 2; i < n; ++i){
				aux = nullptr;
				while(nodeStack.top()->suffixIndex > lcp->at(i)){
					expandNode(nodeStack.top(), aux);
					aux = nodeStack.top();
					nodeStack.pop();
				}
				if(nodeStack.top()->suffixIndex < lcp->at(i)){
					node = new Node(lcp->at(i), false, alphabetSize);
					expandNode(node, aux);
					nodeStack.push(node);
				}
				else{
					expandNode(nodeStack.top(), aux);
				}
			}
			aux = nullptr;
			while(nodeStack.size() > 1){
				expandNode(nodeStack.top(), aux);
				aux = nodeStack.top();
				nodeStack.pop();
			}
			expandNode(nodeStack.top(), aux);
			root = nodeStack.top();
		}

		//build the suffix tree's leaves
		void buildLeaves(Node * & node, unsigned int & index){
			for(unsigned int i = 0; i < node->childrenNumber; ++i){
				if(node->children->at(i) == nullptr){
					Node * leaf = new Node(suffixVector->at(index).second, true);
					leaf->parent = node;
					node->children->at(i) = leaf;
					++index;
				}
				else{
					buildLeaves(node->children->at(i), index);
				}	
			}	
		}

		//update every field from the suffix tree's nodes;
		void updateFields(Node * & node){
			if(node->isLeaf){
				node->right = text.size() - 1;
				if(node->parent != nullptr){
					node->left = node->suffixIndex + node->parent->suffixIndex;
				}
				else{
					node->left = node->right;
				}
				return;
			}
			else{
				unsigned int leavesNumber = 0;
				for(unsigned int i = 0; i < node->childrenNumber; ++i){
					if(node->children->at(i) != nullptr){
						updateFields(node->children->at(i));
						leavesNumber = leavesNumber + node->children->at(i)->leavesNumber;
					}
				}
				node->leavesNumber = leavesNumber;
				
				if(node->parent != nullptr){
					node->right = node->children->at(1)->left - 1;
					node->left = node->right - (node->suffixIndex - node->parent->suffixIndex) + 1;
				}

				Node * aux;
				unsigned int previous = '\0', current;
				unsigned int n = node->childrenNumber;
				
				for(unsigned int i = n - 1; i >= 0; --i){
					aux = node->children->at(i);
					node->children->at(i) = nullptr;
					current = text[aux->left]; 
					node->children->at(current) = aux;
					node->childrenPosition->at(current) = previous;
					previous = current;
					if(i == 0){
						break;
					}
				}
				node->firstChild = (char)previous;
			}	
		}

		//spell the word in the text
		Node * spell(Node * & node, std::string & word){
			if(node->isLeaf){
				if(node->parent != nullptr){
					return node;
				}
				else{
					return nullptr;
				}
			}
			
			unsigned int suffixIndex = node->suffixIndex;
			unsigned int c = word[suffixIndex];
			unsigned int size = word.size();

			Node * child = node->children->at(c);

			if(suffixIndex >= size){
				return node;
			}
			if(child == nullptr){
				return nullptr;	
			}

			unsigned int left = child->left;
			unsigned int right = child->right;
			unsigned int matchings = suffixIndex;

			for(unsigned int i = left; i <= right; ++i){
				if(matchings == size){
					break;
				}
				if(text[i] == word[matchings]){
					++matchings;
				}
				else{
					return nullptr;
				}
			}	
			return spell(child, word);
		}

		/*
		check whether the word contains letters
		out of text alphabet's range. If so, return 
		false, otherwise return true
		*/
		bool checkWord(std::string & word){
			unsigned int n = word.size();
			
			for(unsigned int i = 0; i < n; ++i){
				if((unsigned int) word[i] > alphabetSize){
					return false;
				}
				else if((unsigned int) word[i] < leftInterval){
					return false;
				}
			}

			return true;
		}		
		
		//remake the suffix and lcp vectors from prebuilt suffix tree
		void rebuildVectors(Node * & node){

			if(node->isLeaf){
				this->suffixVectorRemake.push_back(node->suffixIndex);
				return;
			}

			unsigned int n = node->childrenNumber;
			unsigned int c = node->firstChild;
			for(unsigned int i = 0; i < n; ++i){	
				
				rebuildVectors(node->children->at(c));
				c = node->childrenPosition->at(c);
				
				if(i != n - 1){
					lcpRemake.push_back(node->suffixIndex);	
				}
			}
		}

		/*
		call the build function to remake the suffix 
		and lcp vectors from prebuilt suffix tree 
		*/
		void remakeSuffixVectorAndLCP(){

			lcpRemake.push_back(0);
			
			rebuildVectors(this->root);
		}

		/*
		print out the lcp and suffix vector, both 
		computed before building the suffix tree
		*/
		void printBruteForceVectors(){
			unsigned int n = suffixVector->size();	

			std::cout << text << '\n';
			std::cout << "VS: " << suffixVector->at(0).second;
			for(unsigned int i = 1; i < n; ++i){
				std::cout << ' ' << suffixVector->at(i).second;
			}
			std::cout << '\n';

			std::cout << "LCP: " << lcp->at(1);
			for(unsigned int i = 2; i < n; ++i){
				std::cout << ' ' << lcp->at(i);
			}
			std::cout << '\n';
		}

		/*
		print out the lcp and suffix vector, both 
		computed after building the suffix tree
		*/
		void printRemakeVectors(){
			unsigned int n = suffixVectorRemake.size();	

			std::cout << text << '\n';
			std::cout << "VS: " << suffixVectorRemake[0];
			for(unsigned int i = 1; i < n; ++i){
				std::cout << ' ' << suffixVectorRemake[i];
			}
			std::cout << '\n';

			std::cout << "LCP: " << lcpRemake[1];
			for(unsigned int i = 2; i < n; ++i){
				std::cout << ' ' << lcpRemake[i];
			}
			std::cout << '\n';
		}

	public:

		//suffix tree's constructor
		SuffixTree(std::string & text){
			this->text = text + '\0';
			buildVectors();
			unsigned int val = 0;
			buildLeaves(this->root, val);
			updateFields(this->root);
			remakeSuffixVectorAndLCP();
		}

		//suffix tree's destructor
		~SuffixTree(){
			delete root;
			delete lcp;
			delete suffixVector;
		}

		/*
		search for a word in the given text: 
		return true if found, and false otherwise
		*/
		void search(std::string & word){
			if(!checkWord(word)){
				std::cout << "False" << '\n';
				return;
			}

			if(spell(this->root, word) == nullptr){
				std::cout << "False" << '\n'; 
			}
			else{
				std::cout << "True" << '\n';
			}
		}	

		/*
		return a list of occurences' positions of 
		a given word in the text
		*/ 
		std::vector<unsigned int> occurences(std::string & word){
			
			std::vector<unsigned int> occurences;
			
			if(!checkWord(word)){
				return occurences;
			}

			Node * node = spell(this->root, word);

			if(node == nullptr){
				return occurences;
			}
			else{
				getOccurences(occurences, node, 0);
				return occurences;
			}
		}

		//count the number of occurances of a word in the given text
		unsigned int countOccurences(std::string & word){
			
			if(!checkWord(word)){
				return 0;
			}

			Node * node = spell(this->root, word);
			if(node == nullptr){
				return 0;
			}
			else{
				return node->leavesNumber;
			}
		}

		//print out the lcp, suffix vector, and suffix tree in postorder
		void printVS_AS(){
			printBruteForceVectors();
			print(this->root, 0);
		}

		//print out the remade lcp, remade suffix vector, and suffix tree in postorder
		void printAS_VS(){
			print(this->root, 0);
			printRemakeVectors();
		} 

};