#include "newnode.h"

template <typename Key>

//generic treap's class
class Treap{
	private:
		NewNode<Key> * root;

		//rotate left to balance the subtree
		NewNode<Key> * leftRotate(NewNode<Key> * root, NewNode<Key> * x){
	 	   	NewNode<Key> * node = new NewNode<Key>();
	 	   	NewNode<Key> * nodeRight = new NewNode<Key>();
	 	   	
	 	   	node->priority = root->priority;
	 	   	node->key = root->key;
	 	   	node->left = root->left;
	 	   	node->right = x->right->left;

	 	   	nodeRight->key = x->right->key;
	 	   	nodeRight->priority = x->right->priority;
	 	   	nodeRight->left = node;
	 	   	nodeRight->right = x->right->right;
			return nodeRight;
		}

		//rotate right to balance the subtree
		NewNode<Key> * rightRotate(NewNode<Key> * root, NewNode<Key> * x){
			NewNode<Key> * node = new NewNode<Key>();
			NewNode<Key> * nodeLeft = new NewNode<Key>();

			node->priority = root->priority;
			node->key = root->key;
			node->left = x->left->right;
			node->right = root->right;

			nodeLeft->key = x->left->key;
			nodeLeft->priority = x->left->priority;
			nodeLeft->right = node;
			nodeLeft->left = x->left->left;

			return nodeLeft;
		}

		//search for the key recursively
		bool search(NewNode<Key> * root, Key key){
			if(root == nullptr){
				return false;
			}
			if(key < root->key){
				return search(root->left, key);
			}
			else if(key > root->key){
				return search(root->right, key);
			}
			else{
				return true;
			}

		}

		//insert the key into the treap recursively
		NewNode<Key> * insert(NewNode<Key> * root, Key key){
	    	if(root == nullptr){
				return new NewNode<Key>(key);
	    	}

		    NewNode<Key> * node = new NewNode<Key>();

		    if(key < root->key){
				node->key = root->key;
				node->priority = root->priority;
				node->right = root->right;
		        node->left = insert(root->left, key);

		        if(node->left != nullptr && node->left->priority > node->priority){
		            node = rightRotate(root, node);
		        }
		    }
		    else if(key > root->key){
				node->key = root->key;
				node->priority = root->priority;
				node->left = root->left;
		        node->right = insert(root->right, key);

		        if (node->right != nullptr && node->right->priority > node->priority) {
		            node = leftRotate(root, node);
		        }	

		    }
		    else{
				node->key = root->key;
				node->priority = root->priority;
				node->left = root->left;
				node->right = root->right;
		    }
			return node;
		}
		
		//delete the whole treap
		void stopMemoryLeak(NewNode<Key> * & root){
			if (root == nullptr) return;
			if(root->left != nullptr) stopMemoryLeak(root->left);
			if(root->right != nullptr) stopMemoryLeak(root->right);
			delete root;
		}

		//remove the key from the treap recursively
		NewNode<Key> * remove(NewNode<Key> * root, Key key){
			if(root == nullptr){
				return root;
			}
			
			NewNode<Key> * node = new NewNode<Key>();
			if(key < root->key){
				node->key = root->key;
				node->priority = root->priority;
				node->right = root->right;
				node->left = remove(root->left, key);
			}
			else if(key > root->key){
				node->key = root->key;
				node->priority = root->priority;
				node->left = root->left;
				node->right = remove(root->right, key);
			}
			else{
				if(root->left == nullptr && root->right == nullptr){
					node = nullptr;
				}
				else if(root->left != nullptr && root->right != nullptr){
					
					if(root->left->priority < root->right->priority){
						node->right = root->right;
						node->left = root->left;
						node = leftRotate(root, node);
						node->left = remove(node->left, key);
					}
					else{
						node->left = root->left;
						node->right = root->right;
						node = rightRotate(root, node);
						node->right = remove(node->right, key);
					}
				}
				else{	
					
					if(root->left == nullptr){
						return root->right;
					}
					return root->left;
				}
			}
			return node;
		}

		//print inorder traversal of the treap recursively
		void print(NewNode<Key> * root, unsigned int depth){
			
			if(root == nullptr){
				return;
			}
			print(root->left, depth + 3);
			for(unsigned int i = 0; i < depth; i++){
				std::cout << ' ';
			}
			std::cout << root->key << " " << root->priority << '\n';
			print(root->right, depth + 3);
		}
	
	public:

		//treap constructor
		Treap(){
			this->root = nullptr;
		}

		//treap destructor to avoid memory leak
		~Treap(){
			stopMemoryLeak(this->root);
			this->root = nullptr;
		}

		//get the smallest key from the treap
		Key getMin(){
			NewNode<Key> * x = this->root;
			while(x->left != nullptr){
				x = x->left;
			}
			return x->key;
		}

		//print inorder traversal of the treap
		void print(){
			print(this->root, 0);
		}

		//insert a key into the treap
		Treap * insert(Key key){
			Treap * treap = new Treap(); 
			treap->root = insert(this->root, key);
			return treap;
		}

		//search a key from the treap
		bool search(Key key){
			return search(this->root, key);
		}

		//remove a key from the treap
		Treap * remove(Key key){
			Treap * treap = new Treap();
			treap->root = remove(this->root, key);
			return treap;
		}

};
