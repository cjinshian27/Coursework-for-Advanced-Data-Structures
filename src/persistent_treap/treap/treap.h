#include "Node.h"

template <typename Key>

//treap class
class Treap{

	Node<Key> * root = nullptr;

	//rotate left to balance the subtree
	Node<Key> * leftRotate(Node<Key> * root){
 	   	Node<Key> * nodeRight = root->right;
	   	Node<Key> * nodeRightLeft = root->right->left;

	   	nodeRight->left = root;
	 	root->right = nodeRightLeft;

		return nodeRight;
	}

	//rotate right to balance the subtree
	Node<Key> * rightRotate(Node<Key> * node){
		Node<Key> * nodeLeft = node->left;
		Node<Key> * nodeLeftRight = node->left->right;

		nodeLeft->right = node;
		node->left = nodeLeftRight;

		return nodeLeft;
	}

	//search for the key recursively
	bool search(Node<Key> * root, Key key){
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
	void insert(Node<Key> * & root, Node<Key> * & node){
    	if(root == nullptr){
			root = node;
    	}
    	else if(insertion->key > root->key){
    		split(root, node->key, node->left, node->right);
    		root = node;
    	}
    	else{
    		if(node->key >= root->key){
    			insert(root->right, node);
    		}
    		else{
    			insert(root->left, node);
    		}
    	}
	}
	
	//delete the whole treap
	void stopMemoryLeak(Node<Key> * & root){
		if (root == nullptr)
    		return;
		
		stopMemoryLeak(root->left);
		stopMemoryLeak(root->right);
		delete root;
		root = nullptr;
	}

	void split(Key key, Node<Key> * node, Node<Key> * left, Node<Key> * right){
		if(node == nullptr){
			left = nullptr;
			right = nullptr;
		}
		else if(key >= node->key){
			split(key, node->right, node->right, right);
			left = node;
		}
		else{
			split(key, node->left, left, node->left);
			right = node;
		}
	}

	void join(Node<Key> * & root, Node<Key> * & left, Node<Key> * & right){
		if(left != nullptr || right != nullptr){
			if(root == left){
				root = left;
			}
			else{
				root = right;
			}
		}
		else if(left->priority > right->priority){
			join(left->right, left->right, right);
			root = left;
		}
		else{
			join(right->left, left, right->left);
			root = right;
		}
	}

	//remove the key from the treap recursively
	void remove(Node<Key> * & root, Node<Key> * & node){
		if(root->key == node->key){	
			Node<Key> * aux = root;
			merge(node, node->left, node->right);
			delete root;
			root = nullptr;
		}
		else if(node->key < root->key){
			remove(root->left, node);
		}
		else{
			remove(root->right, node);
		}
	}
	
	//print inorder traversal of the treap recursively
	void print(Node<Key> * root, unsigned int depth){
		
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
			Node<Key> * x = this->root;
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
			Node<Key> * node = new Node<Key>(key);
			this->root = insert(node); 
		}

		//search a key from the treap
		bool search(Key key){
			return search(this->root, key);
		}

		//remove a key from the treap
		void remove(Key key){
			Node<Key> * node = new Node<Key>(key);		
			this->root = remove(this->root, node);
		}

};
