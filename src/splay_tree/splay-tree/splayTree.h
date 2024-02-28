template <typename Key>

//splay tree class
class SplayTree{

	private:	
		//splay tree's root node
		Node<Key> * root;
		
		//delete the whole splay tree
		void stopMemoryLeak(Node<Key> * & node){
			if (node == nullptr){
	    		return;
			}
	
			stopMemoryLeak(node->left);
			stopMemoryLeak(node->right);
			delete node;
			node = nullptr;
		}
		
		//get the maximum key from the left subtree
		Node<Key> * maxLeftSubtree(Node<Key> * & node){
			Node<Key> * current = node;
			while(current->right != nullptr){
				current = current->right;
			}
			return current;
		}

		//get the minimum key from the right subtree
		Node<Key> * minRightSubtree(Node<Key> * & node){
			Node<Key> * current = node;
			while(current->left != nullptr){
				current = current->left;
			}
			return current;
		}
		
		//rotate left to balance the subtree
		Node<Key> * leftRotate(Node<Key> * & node){

	 	   	Node<Key> * nodeRight = node->right;
	 	   	Node<Key> * dad = node->parent;

			Node<Key> * nodeRightLeft = node->right->left;

		   	nodeRight->left = node;
		   	node->parent = nodeRight;
		   	
		 	node->right = nodeRightLeft;
		 	if(nodeRightLeft != nullptr){
		 		nodeRightLeft->parent = node;
		 	}
		 	nodeRight->parent = dad;

		 	return nodeRight;
		}

		//rotate right to balance the subtree
		Node<Key> * rightRotate(Node<Key> * & node){
			Node<Key> * nodeLeft = node->left;
	 	   	Node<Key> * dad = node->parent;

			Node<Key> * nodeLeftRight = node->left->right;

			nodeLeft->right = node;
			node->parent = nodeLeft;

			node->left = nodeLeftRight;
			if(nodeLeftRight != nullptr){
				nodeLeftRight->parent = node;
			}
			nodeLeft->parent = dad;

			return nodeLeft;
		}

		//do a splay operation on given node 
		void splay(Node<Key> * & node){

			while(node->parent != nullptr){
				if(node->parent->parent == nullptr){
					if(node == node->parent->left){
						node = rightRotate(node->parent);
					}
					else{
						node = leftRotate(node->parent);
					}
				}
				else{
					Node<Key> * grandGrandParent = node->parent->parent->parent;
					Node<Key> * grandpa = node->parent->parent;
					Node<Key> * aux;
					if(node == node->parent->left && node->parent == node->parent->parent->left){
						aux = rightRotate(node->parent->parent);
						node = rightRotate(aux);
						node->parent = grandGrandParent;

					}
					else if(node == node->parent->right && node->parent == node->parent->parent->right){
						
						aux = leftRotate(node->parent->parent);
						node = leftRotate(aux);
						node->parent = grandGrandParent;
	
					}	
					else if(node == node->parent->right && node->parent == node->parent->parent->left){						
						aux = leftRotate(node->parent);
						aux->parent->left = aux;
						node = rightRotate(aux->parent);
						node->parent = grandGrandParent;
					}
					else{
						aux = rightRotate(node->parent);
						aux->parent->right = aux;
						node = leftRotate(aux->parent);
						node->parent = grandGrandParent;
					}

					if(grandGrandParent != nullptr){
						if(grandGrandParent->right == grandpa){
							grandGrandParent->right = node;
						}
						else{
							grandGrandParent->left = node;
						}
					}

				}
			}
			this->root = node;
		}

		//print out the splay tree
		void print(Node<Key> * node, unsigned int depth){
			if(node == nullptr){
				return;
			}
			
			print(node->left, depth + 3);
			
			for(unsigned int i = 0; i < depth; i++){
				std::cout << ' ';
			}
			std::cout << node->key << '\n';
			
			print(node->right, depth + 3);
		}
	public:
		
		//class' constructor
		SplayTree(){
			root = nullptr;
		}

		SplayTree(Node<Key> * & node){
			root = node;
		}


		~SplayTree(){
			stopMemoryLeak(this->root);
		}

		//insert a node with given key into the splay tree
		void insert(Key key){

			if(this->root == nullptr){
				this->root = new Node<Key>(key);
				return;
			}

			Node<Key> * current = this->root;
			
			while(current != nullptr){
				if(key > current->key){
					if(current->right == nullptr){
						Node<Key> * node = new Node<Key>(key);
						current->right = node;
						node->parent = current;
						splay(node);
						return;
					}
					else{
						current = current->right;
					}
				}
				else if(key < current->key){
					if(current->left == nullptr){
						Node<Key> * node = new Node<Key>(key);
						current->left = node; 
						node->parent = current;
						splay(node);
						return;
					}
					else{
						current = current->left;
					}
				}
				else{
					splay(current);
					return;
				}
			}

		}

		//search for a node with given key on the splay tree
		bool search(Key key){
			Node<Key> * current = this->root;
			Node<Key> * previous = nullptr;

			while(current != nullptr){
				previous = current;
				if(key < current->key){
					current = current->left;
				}
				else if(key > current->key){
					current = current->right;
				}
				else{
					break;
				}
			}

			if(current != nullptr){
				splay(current);
				return true;
			}
			else if(previous != nullptr){
				splay(previous);
			}
			return false;
		}

		//remove a node with given key from the splay tree
		void remove(Key key){
			Node<Key> * current = this->root;
			Node<Key> * previous = nullptr;

			//find the node with the key first
			while(current != nullptr){
				previous = current;
				if(key < current->key){
					current = current->left;
				}
				else if(key > current->key){
					current = current->right;
				}
				else{
					break;
				}
			}

			//if node is found, check their conditions
			if(current != nullptr){
				
				if(current->left == nullptr && current->right == nullptr){
					Node<Key> * parent = current->parent;
					if(parent != nullptr){
						if(parent->right == current){
							delete(current);
							parent->right = nullptr;
						}
						else{
							delete(current);
							parent->left = nullptr;
						}
						splay(parent);
					}
					else{
						delete current;
						current = nullptr;
						this->root = nullptr;
					}
				}
				else if(current->left != nullptr && current->right != nullptr){
					Node<Key> * maxLS = maxLeftSubtree(current->left);
					Node<Key> * parent = maxLS->parent;
					current->setKey(maxLS->key);
					
					if(parent->right == maxLS){
						delete parent->right;
						parent->right = nullptr;
					}
					else{
						delete parent->left;
						parent->left = nullptr;
					}

					splay(parent);
				}
				else if(current->left != nullptr){
					Node<Key> * maxLS = maxLeftSubtree(current->left);
					Node<Key> * parent = maxLS->parent;
					current->setKey(maxLS->key);
					
					if(parent->right == maxLS){
						delete parent->right;
						parent->right = nullptr;
					}
					else{
						delete parent->left;
						parent->left = nullptr;
					}

					splay(parent);
				}
				else{
					Node<Key> * minRS = minRightSubtree(current->right);
					Node<Key> * parent = minRS->parent;	
					current->setKey(minRS->key);

					if(parent->right == minRS){
						delete parent->right;
						parent->right = nullptr;
					}
					else{
						delete parent->left;
						parent->left = nullptr;
					}

					splay(parent);
				}

			}
			//if node is not found, do a splay operation on the last visited
			else if(previous != nullptr){
				splay(previous);
			}
		}

		//return the node with the smallest key 
		Key min(){
			Node<Key> * current = root;
			while(current->left != nullptr){
				current = current->left;
			}
			splay(current);
			return current->key;
		}

		//splay the node whose key is given and print out their subtrees
		std::pair<SplayTree *, SplayTree *> split(Key key){
			
			if(this->root == nullptr){
				return {nullptr, nullptr};
			}
			
			Node<Key> * current = this->root;
			Node<Key> * previous = nullptr;

			while(current != nullptr){
				previous = current;
				if(key < current->key){
					current = current->left;
				}
				else if(key > current->key){
					current = current->right;
				}
				else{
					break;
				}
			}

			if(current != nullptr){
				splay(current);
			}
			else{
				splay(previous);
			}
			Node<Key> * aux = this->root->right;
			this->root->right = nullptr;

			SplayTree * splayTree1 = new SplayTree(this->root);
			SplayTree * splayTree2 = new SplayTree(aux);

			return {splayTree1, splayTree2};
		}		

		//join two given trees
		void join(SplayTree * & thatSplayTree){
			Node<Key> * node = maxLeftSubtree(this->root);
			splay(node);
			node->right = thatSplayTree->root;
		}

		//call the print function
		void print(){
			print(this->root, 0);
		}
};