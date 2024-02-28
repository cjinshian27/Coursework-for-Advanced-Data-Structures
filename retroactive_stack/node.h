
template <typename Key, typename Item>

//node's class
class Node{
    public:

    	int priority;
    	int maxSuffix;
    	int weight;
    	bool isLeaf;
    	Key key;
    	Item item;
    	Node * left;
    	Node * right;
	    

    	Node(){
	        this->priority = rand() % 10000;
	        this->isLeaf = false;
    	}

	    Node(Key key, int weight, Item item){
	        this->key = key;
	        this->item = item;
	        this->weight = weight;
	        this->maxSuffix = weight;
	        this->left = nullptr;
	        this->right = nullptr;
	        this->isLeaf = true;
	    }

	   	Node(Key key, int weight){
	        this->key = key;
	        this->weight = weight;
	        this->maxSuffix = weight;
	        this->left = nullptr;
	        this->right = nullptr;
	        this->isLeaf = true;
	    }

	    void setWeight(){
	    	this->weight = this->left->weight + this->right->weight;
	    }
	    void setMaxSuffix(){
	    	this->maxSuffix = std::max(this->right->maxSuffix, this->right->weight + this->left->maxSuffix);
	    }
};
