#include "node.h"

template <typename Key>

//treap class
class Treap{

	Node<Key> * root = nullptr;

	//search for the key recursively
	bool search(Node<Key> * & node, Key key){
		if(node == nullptr){
			return false;
		}
		if(key < node->key){
			return search(node->left, key);
		}
		else if(key > node->key){
			return search(node->right, key);
		}
		else{
			return true;
		}
	}

	//insert the key into the treap recursively
	void insert(Node<Key> * & node, Key key){
		std::pair<Node<Key> *, Node<Key> *> splitTreap = split(node, key);
		if(search(splitTreap.second, key) == false){
			Node<Key> * newNode = new Node<Key>(key);
			Node<Key> * result = join(newNode, splitTreap.second);
			this->root = join(splitTreap.first, result); 
		}
		else{
			this->root = join(splitTreap.first, splitTreap.second);
		}
	}
	
	//delete the whole treap
	void stopMemoryLeak(Node<Key> * & node){
		if (node == nullptr){
    		return;
		}

		stopMemoryLeak(node->left);
		stopMemoryLeak(node->right);
		delete node;
		node = nullptr;
	}

	//split the treap rooted at node based on given key
	std::pair<Node<Key> *, Node<Key> *> split(Node<Key> * & node, Key key, bool isEqual = false){
		if(node == nullptr){
			return {nullptr, nullptr};
		}
		else if(key > node->key || (isEqual && node->key == key)){
			std::pair<Node<Key> *, Node<Key> *> splitTreap = split(node->right, key, isEqual); 
			node->right = splitTreap.first;
			return {node, splitTreap.second};
		}
		else{
			std::pair<Node<Key> *, Node<Key> *> splitTreap = split(node->left, key, isEqual);
			node->left = splitTreap.second;
			return {splitTreap.first, node};
		}
	}

	//join two subtrees rooted at left and right
	Node<Key> * join(Node<Key> * & left, Node<Key> * & right){
		if(left == nullptr){
			return right;
		}
		if(right == nullptr){
			return left;
		}
		if(left->priority > right->priority){
			left->right = join(left->right, right);
			return left;
		}
		else{
			right->left = join(left, right->left);
			return right;
		}
	}

	//remove the key from the treap recursively
	void remove(Node<Key> * & node, Key key){
		std::pair<Node<Key> *, Node<Key> *> splitTreap = split(node, key);
		
		if(splitTreap.second != nullptr){
			std::pair<Node<Key> *, Node<Key> *> rightSplit = split(splitTreap.second, key, true);
			
			//key not found			
			if(rightSplit.first == nullptr){
				this->root = join(splitTreap.first, rightSplit.second);
				return;
			}
			
			delete rightSplit.first;
			rightSplit.first = nullptr;
			this->root = join(splitTreap.first, rightSplit.second);
		}
		else{
			this->root = join(splitTreap.first, splitTreap.second);
		}
	}
	
	//print inorder traversal of the treap recursively
	void print(Node<Key> * & node, unsigned int depth){
		
		if(node == nullptr){
			return;
		}
		print(node->left, depth + 3);
		for(unsigned int i = 0; i < depth; i++){
			std::cout << ' ';
		}
		std::cout << node->key << " " << node->priority << '\n';
		print(node->right, depth + 3);
	}
	
	public:

		//treap destructor to avoid memory leak
		~Treap(){
			stopMemoryLeak(this->root);
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
			insert(this->root, key); 
		}

		//search a key from the treap
		bool search(Key key){
			return search(this->root, key);
		}

		//remove a key from the treap
		void remove(Key key){
			remove(this->root, key);
		}
};
