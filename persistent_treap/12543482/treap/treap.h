#include "newnode.h"

template <typename Key>

//treap class
class Treap{

	NewNode<Key> * root = nullptr;

	//rotate left to balance the subtree
	NewNode<Key> * leftRotate(NewNode<Key> * root){
 	   	NewNode<Key> * nodeRight = root->right;
	   	NewNode<Key> * nodeRightLeft = root->right->left;

	   	nodeRight->left = root;
	 	root->right = nodeRightLeft;

		return nodeRight;
	}

	//rotate right to balance the subtree
	NewNode<Key> * rightRotate(NewNode<Key> * node){
		NewNode<Key> * nodeLeft = node->left;
		NewNode<Key> * nodeLeftRight = node->left->right;

		nodeLeft->right = node;
		node->left = nodeLeftRight;

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

	    if(key < root->key){
	        root->left = insert(root->left, key);

	        if(root->left != nullptr && root->left->priority > root->priority){
	            root = rightRotate(root);
	        }
	    }
	    else if(key > root->key){
	        root->right = insert(root->right, key);

	        if (root->right != nullptr && root->right->priority > root->priority) {
	            root = leftRotate(root);
	        }	

	    }
	    else{
	    	root->key = key;
	    }
		return root;
	}
	
	//delete the whole treap
	void stopMemoryLeak(NewNode<Key> * root){
		if (root == nullptr)
    		return;
		
		if(root->left != nullptr) stopMemoryLeak(root->left);
		if(root->right != nullptr) stopMemoryLeak(root->right);
		delete root;
	}

	//remove the key from the treap recursively
	NewNode<Key> * remove(NewNode<Key> * root, Key key){
		if(root == nullptr){
			return root;
		}
		if(key < root->key){
			root->left = remove(root->left, key);
		}
		else if(key > root->key){
			root->right = remove(root->right, key);
		}
		else{
			if(root->left == nullptr && root->right == nullptr){
				delete root;
				root = nullptr;
			}
			else if(root->left && root->right){
				if(root->left->priority < root->right->priority){
					root = leftRotate(root);
					root->left = remove(root->left, key);
				}
				else{
					root = rightRotate(root);
					root->right = remove(root->right, key);
				}
			}
			else{
				NewNode<Key> * child = (root->left)? root->left : root->right;
				NewNode<Key> * toRemove = root;
				root = child;
				delete toRemove;
			}
		}
		return root;
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
		void insert(Key key){
			this->root = insert(this->root, key); 
		}

		//search a key from the treap
		bool search(Key key){
			return search(this->root, key);
		}

		//remove a key from the treap
		void remove(Key key){
			this->root = remove(this->root, key);
		}

};
