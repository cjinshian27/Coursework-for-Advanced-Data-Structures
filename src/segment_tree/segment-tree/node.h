
class Node{
	public:
		int left;
		bool isLeftClosed;
		bool isRightClosed;
		int right;
		Node * leftChild;
		Node * rightChild;
		int id;
		std::vector<Node *> setOfIntervals;

		Node(){

		}

		Node(int left, int right, bool isLeftClosed, bool isRightClosed){
			this->left = left;
			this->right = right;
			this->isLeftClosed = isLeftClosed;
			this->isRightClosed = isRightClosed;
			this->leftChild = nullptr;
			this->rightChild = nullptr;
		}
};