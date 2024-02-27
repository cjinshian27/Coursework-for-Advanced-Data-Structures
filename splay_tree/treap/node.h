template <typename Key>

class Node{
    public:

    	int priority;
    	Key key;
    	Node * left;
    	Node * right;
	    
	    Node(Key key){
	        this->key = key;
	        this->priority = rand() % 100000;
	        this->left = nullptr;
	        this->right = nullptr;
	    }
};
