 #include "node.h"

template <typename Key, typename Item>

//treap class
class Treap{

	private:
		Node<Key, Item> * root = nullptr;

		//rotate left to balance the subtree
		Node<Key, Item> * leftRotate(Node<Key, Item> * root){
	 	   	Node<Key, Item> * nodeRight = root->right;
		   	Node<Key, Item> * nodeRightLeft = root->right->left;

		   	nodeRight->left = root;
		 	root->right = nodeRightLeft;

			root->setWeight();
			root->setMaxSuffix();
			root->key = root->left->key;
			return nodeRight;
		}

		//rotate right to balance the subtree
		Node<Key, Item> * rightRotate(Node<Key, Item> * root){
			Node<Key, Item> * nodeLeft = root->left;
			Node<Key, Item> * nodeLeftRight = root->left->right;

			nodeLeft->right = root;
			root->left = nodeLeftRight;

			root->setWeight();
			root->setMaxSuffix();
			root->key = root->left->key;
			return nodeLeft;
		}

		//insert a push into the treap at key time
		Node<Key, Item> * insertPush(Node<Key, Item> * root, Key key, Item item){
	    	if(root == nullptr){
				return new Node<Key, Item>(key, 1, item);
	    	}
	    	else if(root->isLeaf){
	    		
	    		Node<Key, Item> * node = new Node<Key, Item>();
	    		
	    		if(key < root->key){
	    			node->left = insertPush(root->left, key, item);
	    			node->right = root;
	    		}
	    		else{
	    			node->left = root;
	    			node->right = insertPush(root->right, key, item);
	    		}
				
				node->key = node->left->key;
				node->setWeight(); 
				node->setMaxSuffix();
	    		return node;
	    	}
		    else if(key < root->right->key){
		        root->left = insertPush(root->left, key, item);

		        if(root->left != nullptr && root->left->priority > root->priority){
		            root = rightRotate(root);
		        }
		    }
		    else{
		        root->right = insertPush(root->right, key, item);

		        if (root->right != nullptr && root->right->priority > root->priority) {
		            root = leftRotate(root);
		        }	
		    }
		 	root->setWeight();
		 	root->setMaxSuffix();

		 	root->key = root->left->key;

			return root;
		}

		//insert a pop into the treap at key time
		Node<Key, Item> * insertPop(Node<Key, Item> * root, Key key){
	    	
	    	if(root == nullptr){
				return new Node<Key, Item>(key, -1);
	    	}
	    	
	    	else if(root->isLeaf){
	    		
	    		Node<Key, Item> * node = new Node<Key, Item>();

	    		if(key < root->key){
	    			node->left = insertPop(root->left, key);
	    			node->right = root;
	    		}
	    		else{
	    			node->left = root;
	    			node->right = insertPop(root->right, key);
	    		}
				node->setWeight(); 
				node->setMaxSuffix();
				node->key = node->left->key;
	    		return node;
	    	}

		    else if(key < root->right->key){

		        root->left = insertPop(root->left, key);

		        if(root->left != nullptr && root->left->priority > root->priority){
		            root = rightRotate(root);
		        }
		    }
		    else{
		        root->right = insertPop(root->right, key);

		        if (root->right != nullptr && root->right->priority > root->priority) {
		            root = leftRotate(root);
		        }	
		    }

			root->setWeight();
		 	root->setMaxSuffix();

		 	root->key = root->left->key;

			return root;
		}
		

		//remove the operation from the treap at key time
		Node<Key, Item> * remove(Node<Key, Item> * root, Key key){
			if(root == nullptr){
				return root;
			}
			else if(root->left->isLeaf && root->left->key == key){
				delete root->left;
				root->left = nullptr;
				return root->right;
			}
			else if(root->right->isLeaf && root->right->key == key){
				delete root->right;
				root->right = nullptr;
				return root->left;
			}
			else if(key < root->right->key){
				root->left = remove(root->left, key);
			}
			else{
				root->right = remove(root->right, key);
			}

		 	root->setWeight();
		 	root->setMaxSuffix();

		 	root->key = root->left->key;

			return root;
		}

		//get the k-th element from a node
		std::pair<Node<Key, Item> *, int> getItem(Node<Key, Item> * root, int k){
			if(root->isLeaf){
				return {root, 0};
			}
			else if(root->right->maxSuffix >= k){
				return getItem(root->right, k);
			}
			else{
				return getItem(root->left, k - root->right->weight);
			}
		}

		//get the k-th element at key time
		std::pair<Node<Key, Item> *, int> k_th(Node<Key, Item> * root, Key key, int k){
			if(root->isLeaf){
				if(root->weight == -1 || k != 1){
					return {nullptr, root->weight};
				}
				else{
					return getItem(root, k);
				}
			}
			else if(key < root->right->key){
				return k_th(root->left, key, k);
			}
			else{
				std::pair<Node<Key, Item> *, int> pair = k_th(root->right, key, k);
				
				if(pair.first == nullptr){
					
					k -= pair.second;
					
					if(root->left->maxSuffix >= k){
						return getItem(root->left, k);
					}
					else{
						return {nullptr, pair.second + root->left->weight};
					}	
				}
				else{
					return pair;
				}
			}

		}

		//return the size at key time
		int size(Node<Key, Item> * root, Key key){
			if(root->isLeaf){
				return root->weight;
			}
			else if(key < root->right->key){
				return size(root->left, key);
			}
			else{
				return root->left->weight + size(root->right, key);  
			}
		}

		//print the treap up to k-th element at key time
		void print(Node<Key, Item> * root, Key key){
			std::cout << k_th(root, key, 1).first->item;
			
			int s = size(root, key);

			for(int i = 2; i <= s; i++){
				std::cout << ' ' << k_th(root, key, i).first->item;
			}
			std::cout << '\n';
		}
		
		//print the treap recursively for debugging
		void debug(Node<Key, Item> * root, int space, int count){

	 	   	if (root == nullptr)
	 	   	    return;
	 	
	 	   	space += count;
	 	
	 	   	debug(root->right, space, count);
	 	
	 	   	std::cout << '\n';
	 	   	for (int i = count; i < space; i++)
	 	   	    std::cout << " ";
	 	   	if(root->weight == -1 && root->isLeaf){
	 	   		std::cout << "@" << "\n";
	 	   	}
	 	   	else{
	 	   		std::cout << root->item << " key:" << root->key <<  "\n";
	 	   	}
	 	
	    	debug(root->left, space, count);
		}


		//delete the whole treap
		void stopMemoryLeak(Node<Key, Item> * & root){
			if (root == nullptr)
	    		return;
			
			stopMemoryLeak(root->left);
			stopMemoryLeak(root->right);
			delete root;
			root = nullptr;
		}

	public:

		//treap destructor to avoid memory leak
		~Treap(){
			stopMemoryLeak(this->root);
		}

		//return the k_th element at key time 
		Item k_th(Key key, int k){
			return k_th(this->root, key, k).first->item;
		}

		//print the top at key time
		Item top(Key key){
			return k_th(key, 1);
		}

		//print the treap at key time
		void print(Key key){
			//debug(this->root, 0, 10);
			//std::cout << "\n\n\n";
			print(this->root, key);
		}

		//insert a push into the treap at key time
		void insertPush(Key key, Item item){
			this->root = insertPush(this->root, key, item);
		}

		//insert a pop into the treap at key time
		void insertPop(Key key){
			this->root = insertPop(this->root, key); 
		}

		//return the size at key time
		int size(Key key){
			return size(this->root, key);
		}

		//remove the operation at key time
		void remove(Key key){
			this->root = remove(this->root, key);
		}
};
