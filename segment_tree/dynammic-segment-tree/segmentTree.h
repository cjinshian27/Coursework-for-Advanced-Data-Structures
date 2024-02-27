#include "node.h"

//SegmentTree's class
class SegmentTree{
	
	private:
		std::vector<Node *> leaves;
		std::vector<int> sortedEndpoints;	
		Node * root;	

		//delete the entire segment tree
		void stopMemoryLeak(Node * & root){
			if(root == nullptr) return;
			stopMemoryLeak(root->leftChild);
			stopMemoryLeak(root->rightChild);

			delete root;
			root = nullptr;
		}

		//check if first node intersects second node
		bool intersect(Node * & first, Node * & second){
			if(second->right < first->left || first->right < second->left){
				return false;
			}
			else if(second->right == first->left || first->right == second->left){
				if((second->isRightClosed && first->isLeftClosed) || (first->isRightClosed && second->isLeftClosed)){
					return true;
				}
				return false;
			}
			return true;
		}

		//check if first node is contained in second node
		bool contain(Node * & first, Node * & second){

			if((second->left <= first->left) && (second->right >= first->right)){
				if(second->left == first->left || second->right == first->right){
					if(second->left == first->left && second->right == first->right){
						if((second->isLeftClosed && second->isRightClosed) || 
							(second->isLeftClosed == first->isLeftClosed && second->isRightClosed == first->isRightClosed)){
								return true;
						}
					}
					else if(second->left == first->left){

						if(second->isLeftClosed || (second->isLeftClosed == first->isLeftClosed)){
							return true;
						}
					}
					else if(second->right == first->right){
						if(second->isRightClosed || (second->isRightClosed == first->isRightClosed)){
							return true;
						}
					}
					return false;
				}
				return true;				
			}
			return false;
		}

		//class' destructor
	public:
		std::vector<Node *> intervals;
		std::vector<Node *> containedSegments;
		
		//class' destructor
		~SegmentTree(){
			stopMemoryLeak(this->root);
		}

		//class' constructor
		SegmentTree(std::vector<Node *> & intervals){
			this->intervals = intervals;
			unsigned int n = intervals.size();
			sortedEndpoints.push_back(INT_MIN);
			sortedEndpoints.push_back(INT_MAX);

			for(unsigned int i = 0; i < n; i++){
				sortedEndpoints.push_back(intervals[i]->left);
				sortedEndpoints.push_back(intervals[i]->right);
			}
			std::sort(sortedEndpoints.begin(), sortedEndpoints.end());
			sortedEndpoints.erase(std::unique( sortedEndpoints.begin(), sortedEndpoints.end() ), sortedEndpoints.end() );
			for(unsigned int i = 0; i + 1 < sortedEndpoints.size(); i++){
				Node * node1 = new Node(sortedEndpoints[i], sortedEndpoints[i + 1], false, false);
				Node * node2 = new Node(sortedEndpoints[i + 1], sortedEndpoints[i + 1], true, true);
				this->leaves.push_back(node1);
				this->leaves.push_back(node2);	
			}
			leaves.pop_back();
		}

		void getSegments(Node * & root, Node * & x){
			
			if(root->leftChild == nullptr && root->rightChild == nullptr){
				if(root->setOfIntervals.size() > 0){
					for(unsigned int i = 0; i < root->setOfIntervals.size(); ++i){
						containedSegments.push_back(root->setOfIntervals[i]);
					}
				}
				return;
				
			}
			bool containsLeft = contain(x, root->leftChild);
			bool containsRight = contain(x, root->rightChild);

			if(containsLeft && containsRight){
				if(root->setOfIntervals.size() > 0){
					for(unsigned int i = 0; i < root->setOfIntervals.size(); ++i){
						containedSegments.push_back(root->setOfIntervals[i]);
					}
				}
				return;
			}
			else{
				if(containsLeft){
					if(root->setOfIntervals.size() > 0){
						for(unsigned int i = 0; i < root->setOfIntervals.size(); ++i){
							containedSegments.push_back(root->setOfIntervals[i]);
						}
					}
					getSegments(root->leftChild, x);
				}
				else{
					if(root->setOfIntervals.size() > 0){
						for(unsigned int i = 0; i < root->setOfIntervals.size(); ++i){
							containedSegments.push_back(root->setOfIntervals[i]);
						}
					}
					getSegments(root->rightChild, x);
				}
			}

		}

		//get the segments whose x are contained in
		void segments(Node * & root, Node * & x){
			getSegments(root, x);
		}

		//insert element (segment) into the segment tree
		void insert(Node * & root, Node * & element){
			if(root->leftChild == nullptr && root->rightChild == nullptr){

				root->setOfIntervals.push_back(element);

				return;
			}
			bool containsLeft = contain(root->leftChild, element);
			bool containsRight = contain(root->rightChild, element);
			bool intersectLeft = intersect(root->leftChild, element);
			bool intersectRight = intersect(root->rightChild, element); 

			if(containsLeft && containsRight){
				root->setOfIntervals.push_back(element);	
			}
			else{
				if(intersectLeft){
					insert(root->leftChild, element);
				}
				if(intersectRight){
					insert(root->rightChild, element);
				}
			}
		}

		//build the segment tree
		void build(){	
			
			if(this->leaves.size() < 3){
				return;
			}
			unsigned int n = this->leaves.size();

			unsigned int height = floor(log2(n));
			unsigned int layer = pow(2, height);

			unsigned int diff = 2 * (n - pow(2, height));

			unsigned int i = 0;
			
			std::vector<Node *> aux;
			int leftValue, rightValue;
			bool leftBracket, rightBracket;

			for(i = 0; i + 1 < diff; i += 2){
				leftValue = this->leaves[i]->left;
				rightValue = this->leaves[i + 1]->right;
				leftBracket = this->leaves[i]->isLeftClosed;
				rightBracket = this->leaves[i + 1]->isRightClosed;

				Node * node = new Node(leftValue, rightValue, leftBracket, rightBracket);
				node->leftChild = this->leaves[i];
				node->rightChild = this->leaves[i + 1];
				aux.push_back(node);
			}		

			for(; i < n; i++){
				aux.push_back(leaves[i]);
			}	

			unsigned int j = 0;
			unsigned int k = 0;
			while(j < height){

				for(i = 0; i + 1 < layer; i += 2){
					leftValue = aux[i]->left;
					rightValue = aux[i + 1]->right;
					leftBracket = aux[i]->isLeftClosed;
					rightBracket = aux[i + 1]->isRightClosed;
					Node * node = new Node(leftValue, rightValue, leftBracket, rightBracket);
					node->leftChild = aux[i];
					node->rightChild = aux[i + 1];
					aux[k++] = node;
				}
				k = 0;
				layer = layer / 2; 

				++j;
			}
			this->root = aux[0];

			for(Node * & element : this->intervals){
				insert(this->root, element);
			}
		}

		//print out the segment tree for debugging purposes
		void debug(Node * root, int space, int count){
				
			if(root == nullptr) return;

			space += count;

			debug(root->rightChild, space, count);

			std::cout << "\n";
			for (int i = count; i < space; i++) std::cout << " ";

			if(root->isLeftClosed){
	    	std::cout << '[';	
		    }
			else{
				std::cout << '(';
			}

			if(root->left == INT_MIN && root->right == INT_MAX){
				std::cout << "-inf,inf";
			}
			else if(root->left == INT_MIN){
				std::cout << "-inf" << "," << root->right;
			}
			else if(root->right == INT_MAX){
				std::cout << root->left << "," << "inf";
			}
			else{
				std::cout << root->left << "," << root->right;
			}

			if(root->isRightClosed){
				std::cout << ']';
			}
			else{
				std::cout << ')';
			}		
			if(root->setOfIntervals.size() > 0){
				std::cout << ' ' << root->setOfIntervals[0]->id;
				for(unsigned int i = 1; i < root->setOfIntervals.size(); ++i){
					std::cout << ' ' << root->setOfIntervals[i]->id;
				}
			}
			std::cout << '\n';

			debug(root->leftChild, space, count);
		}

		//print out the segment tree
		void print(Node * & root, unsigned int depth){

			if(root == nullptr){
				return;
			}
			print(root->leftChild, depth + 3);

			for(unsigned int i = 0; i < depth; i++){
				std::cout << ' ';
			}
			if(root->isLeftClosed){
	    		std::cout << '[';	
		    }
			else{
				std::cout << '(';
			}

			if(root->left == INT_MIN && root->right == INT_MAX){
				std::cout << "-inf,+inf";
			}
			else if(root->left == INT_MIN){
				std::cout << "-inf" << "," << root->right;
			}
			else if(root->right == INT_MAX){
				std::cout << root->left << "," << "+inf";
			}
			else{
				std::cout << root->left << "," << root->right;
			}

			if(root->isRightClosed){
				std::cout << ']';
			}
			else{
				std::cout << ')';
			}		
			if(root->setOfIntervals.size() > 0){
				for(unsigned int i = 0; i < root->setOfIntervals.size(); ++i){
					std::cout << ' ' << root->setOfIntervals[i]->id;
				}
			}
			std::cout << '\n';
			print(root->rightChild, depth + 3);
		}


		//call debug function
		void debug(){
			debug(this->root, 0, 8);
		}

		//call the print function
		void print(){
			print(this->root, 0);
		}

		//call segments function
		void segments(int x){
			Node * node = new Node(x, x, true, true);
			segments(this->root, node);
			delete node; 
			node = nullptr;
		}

};