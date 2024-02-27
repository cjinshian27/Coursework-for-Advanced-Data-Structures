#include "edge.h"

template <typename Key>

//splay tree class
class SplayTree{

	private:	

		//get the size of an edge
		unsigned int size(Edge<Key> * & edge){
			if(edge == nullptr){
				return 0;
			}
			else{
				return edge->size;
			}
		}

		//delete the whole splay tree
		void stopMemoryLeak(Edge<Key> * & edge){
			if (edge == nullptr){
	    		return;
			}
	
			stopMemoryLeak(edge->left);
			stopMemoryLeak(edge->right);
			delete edge;
			edge = nullptr;
		}
		
		//get the maximum key from the left subtree
		Edge<Key> * maxLeftSubtree(Edge<Key> * & edge){
			Edge<Key> * current = edge;
			while(current->right != nullptr){
				current = current->right;
			}
			return current;
		}

		//get the minimum key from the right subtree
		Edge<Key> * minRightSubtree(Edge<Key> * & edge){
			Edge<Key> * current = edge;
			while(current->left != nullptr){
				current = current->left;
			}
			return current;
		}
		
		//rotate left to balance the subtree
		Edge<Key> * leftRotate(Edge<Key> * & edge){
	 	   	Edge<Key> * nodeRight = edge->right;
	 	   	Edge<Key> * dad = edge->parent;
			Edge<Key> * nodeRightLeft = edge->right->left;

		   	nodeRight->left = edge;
		   	edge->parent = nodeRight;
		   	
		 	edge->right = nodeRightLeft;
		 	if(nodeRightLeft != nullptr){
		 		nodeRightLeft->parent = edge;
		 	}
		 	edge->setSize();
		 	nodeRight->parent = dad;

		 	return nodeRight;
		}

		//rotate right to balance the subtree
		Edge<Key> * rightRotate(Edge<Key> * & edge){
			Edge<Key> * nodeLeft = edge->left;
	 	   	Edge<Key> * dad = edge->parent;

			Edge<Key> * nodeLeftRight = edge->left->right;

			nodeLeft->right = edge;
			edge->parent = nodeLeft;

			edge->left = nodeLeftRight;
			if(nodeLeftRight != nullptr){
				nodeLeftRight->parent = edge;
			}
			edge->setSize();
			nodeLeft->parent = dad;

			return nodeLeft;
		}
		
		//print out the splay tree
		void printTree(Edge<Key> * & edge){
			if(edge == nullptr){
				return;
			}
			
			printTree(edge->left);
			
			edge->print();

			printTree(edge->right);
		}

		//do a splay operation on given edge 
		void splay(Edge<Key> * & edge){
			while(edge->parent != nullptr){
				if(edge->parent->parent == nullptr){
					if(edge == edge->parent->left){
						edge = rightRotate(edge->parent);
					}
					else{
						edge = leftRotate(edge->parent);						
					}
				}
				else{
					Edge<Key> * grandGrandParent = edge->parent->parent->parent;
					Edge<Key> * grandpa = edge->parent->parent;
					Edge<Key> * aux;
					if(edge == edge->parent->left && edge->parent == edge->parent->parent->left){
						aux = rightRotate(edge->parent->parent);
						edge = rightRotate(aux);
						edge->parent = grandGrandParent;
					}
					else if(edge == edge->parent->right && edge->parent == edge->parent->parent->right){
						
						aux = leftRotate(edge->parent->parent);
						edge = leftRotate(aux);
						edge->parent = grandGrandParent;
					}	
					else if(edge == edge->parent->right && edge->parent == edge->parent->parent->left){						
						aux = leftRotate(edge->parent);
						aux->parent->left = aux;
						edge = rightRotate(aux->parent);
						edge->parent = grandGrandParent;
					}
					else{
						aux = rightRotate(edge->parent);
						aux->parent->right = aux;
						edge = leftRotate(aux->parent);
						edge->parent = grandGrandParent;
					}
					
					edge->setSize();
					
					if(grandGrandParent != nullptr){
						if(grandGrandParent->right == grandpa){
							grandGrandParent->right = edge;
						}
						else{
							grandGrandParent->left = edge;
						}
						grandGrandParent->setSize();
					}
				}
			}
			this->root = edge;
			
			this->root->setSize();
		}
		
		//operator < overloading for splay tree comparison
		bool operator < (const SplayTree & that){
			return this->root->id < that->root->id;
		}
		
		//operator > overloading for splay tree comparison
		bool operator > (const SplayTree & that){
			return this->root->id > that->root->id;
		}
		
		//operator = overloading for splay tree's comparison
		bool operator = (const SplayTree & that){
			return this->root->id == that->root->id;
		}

	public:

		//splay tree's root edge
		Edge<Key> * root = nullptr;
		
		//class' constructor
		SplayTree(){
		}

		//class' parameterized constructor
		SplayTree(Key u, Key v, unsigned int id){
			root = new Edge<Key>(u, v, id);
		}

		//class parameterized constructor
		SplayTree(Edge<Key> * & edge){
			root = edge;
		}

		//class' destructor
		~SplayTree(){
			stopMemoryLeak(this->root);
		}

		//return the k-th edge from edge
		Edge<Key> * k_th(Edge<Key> * & edge, unsigned int k){
			unsigned int value = size(edge->left) + 1;
			if(value == k){
				return edge;
			}
			else if(value > k){	
				return k_th(edge->left, k);
			}
			else{
				return k_th(edge->right, k - value);
			}
		}

		//splay the edge whose key is given and print out their subtrees
		std::pair<SplayTree *, SplayTree *> split(unsigned int k){

			if(!k){
				return {nullptr, this};
			}

			Edge<Key> * aux = k_th(this->root, k);
			splay(aux);

			aux = this->root->right;
			
			if(aux != nullptr){
				aux->parent = nullptr;
			}


			this->root->right = nullptr;
			this->root->setSize();

			SplayTree<Key> * splayTree1 = new SplayTree<Key>(this->root);
			SplayTree<Key> * splayTree2 = new SplayTree<Key>(aux);
			if(aux == nullptr){
				return {splayTree1, nullptr};
			}

			return {splayTree1, splayTree2};
		}		

		//join two given trees
		void join(SplayTree * & thatSplayTree){
			
			Edge<Key> * edge = maxLeftSubtree(this->root);
			splay(edge);

			if(thatSplayTree == nullptr){
				this->root->right = nullptr;
			}
			else{
				this->root->right = thatSplayTree->root;
				this->root->right->parent = this->root;
			}
			this->root->setSize();
		}

		//balance the tree by splaying given edge
		void balance(Edge<Key> * & edge){
			if(edge == nullptr) return;
			splay(edge);
		}

		//call the print function
		void print(){
			printTree(this->root);
		}
};