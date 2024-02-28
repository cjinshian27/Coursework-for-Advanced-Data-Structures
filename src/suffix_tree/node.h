//node's class
class Node{

	public:
		bool isLeaf;
		char firstChild;
		unsigned int left;
		unsigned int right;
		unsigned int suffixIndex;
		unsigned int leavesNumber;
		unsigned int childrenNumber;

		Node * parent = nullptr;
		
		std::vector<Node *> * children;
		std::vector<char> * childrenPosition;

		//node's parameterized constructor
		Node(unsigned int suffixIndex, bool isLeaf, unsigned int size = 0){
			this->left = 0;
			this->right = 0;
			this->isLeaf = isLeaf;
			this->firstChild = '\0';
			this->childrenNumber = 0;
			this->suffixIndex = suffixIndex;

			if(isLeaf){
				children = nullptr;
				childrenPosition = nullptr;
				this->leavesNumber = 1;
			}
			else{
				children = new std::vector<Node *>(size + 1);
				childrenPosition = new std::vector<char>(size + 1, '\0');
				this->leavesNumber = 0;
			}
		}	

		//node's destructor
		~Node(){
			for(unsigned int i = 0; i < childrenNumber; ++i){
				delete children->at(i);
				children->at(i) = nullptr;
			}
			delete childrenPosition;
			childrenPosition = nullptr;
		}

		//print out node's fields
		void print(){
			std::cout << this->suffixIndex << '\n';
		}
};