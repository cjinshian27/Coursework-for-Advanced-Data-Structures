
//Node's class
class Node{
	
	public:
		int left;
		int right;
		bool isLeftClosed;
		bool isRightClosed;
		Node * leftChild;
		Node * rightChild;
		int id;
		std::vector<Node *> setOfIntervals;

		//class' default constructodr
		Node(){

		}	

		//class' parametrized constructor
		Node(int left, int right, bool isLeftClosed, bool isRightClosed){
			this->left = left;
			this->right = right;
			this->isLeftClosed = isLeftClosed;
			this->isRightClosed = isRightClosed;
			this->leftChild = nullptr;
			this->rightChild = nullptr;
		}

};