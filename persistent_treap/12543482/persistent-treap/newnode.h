
template <typename Key>
class NewNode{
    public:

    	Key key;
    	int priority;
    	NewNode * left;
    	NewNode * right;
	    
    	NewNode(){

    	}

	    NewNode(Key key){
	        this->key = key;
	        this->priority = rand() % 10000;
	        this->left = nullptr;
	        this->right = nullptr;
	    }
};
