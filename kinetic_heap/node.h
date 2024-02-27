//node's (element) class
class Node{

	public:
		int id;
		int index;
		double x0;
		double velocity;
		
		//node default constructor
		Node(){

		}

		//node parameterized constructor
		Node(double id, double x0, double velocity){
			this->id = id;
			this->x0 = x0;
			this->velocity = velocity;
		}		
};