template<typename Key>

//edge's class
class Edge{
	
	public:

		unsigned int id;
		std::pair<Key, Key> edge;
		unsigned int size = 1;
		Edge * left = nullptr;
		Edge * right = nullptr;
		Edge * parent = nullptr;

		//class' parameterized constructor
		Edge(Key first, Key second, unsigned int id){
			this->edge.first = first;
			this->edge.second = second;
			this->id = id;
		}

		//print the edge in the format (u, v)
		void print(){

			std::cout << this->edge.first << " " << this->edge.second << '\n';
		}

		//set the edge's size
		void setSize(){
			if(this->left != nullptr && this->right != nullptr){
				this->size = 1 + this->left->size + this->right->size;
			}
			else if(this->left != nullptr){
				this->size = 1 + this->left->size;
			}
			else if(this->right != nullptr){
				this->size = 1 + this->right->size;
			}
			else{
				this->size = 1;
			}
		}

};