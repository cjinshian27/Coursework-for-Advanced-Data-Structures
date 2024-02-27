//certificate's class
class Certificate{
	
	public:
		int index;
		int position;
		double time;

		//certificate's default constructor
		Certificate(){

		}

		//certificate's constructor
		Certificate(int position, double time){
			this->position = position;
			this->time = time;
		}
};		