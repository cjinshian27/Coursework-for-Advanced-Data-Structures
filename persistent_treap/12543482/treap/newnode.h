
template <typename Key>
class NewNode{
    public:

    	int priority;
    	Key key;
    	NewNode * left;
    	NewNode * right;
	    
	    NewNode(Key key){
	        this->key = key;
	        this->priority = rand() % 10000;
	        this->left = nullptr;
	        this->right = nullptr;
	    }
};
