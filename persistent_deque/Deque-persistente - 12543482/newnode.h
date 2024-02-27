/*********************************************************************************

	          						CLASS NEWNODE

**********************************************************************************/

template<typename Item> 

class NewNode{
	public:
		Item val;
		NewNode<Item> * parent = nullptr;
		NewNode<Item> * jump = nullptr;
		unsigned int depth = 0;
		
		//newnode's default constructor
		NewNode(){
			this->jump = this;
			this->jump->depth = 0;
			this->parent = this;
		}

		//newnode's parameterized constructor
		NewNode(Item val, NewNode<Item> * parent, unsigned int depth){
			this->val = val;
			this->parent = parent;
			this->depth = depth;
		}	
};