
//kinetic heap's class
class KineticHeap{
	private:
		double now = 0;
		//hash function to map id to node pointer
		std::unordered_map<int, Node *> hashMap;
		//hash function to map element index to its corresponding certificate
		std::unordered_map<int, Certificate *> node2certificate;

		MinPriorityQueue * certificates;
		std::vector<Node *> maxHeap;
		
		//parent of element i
		int parent(int i){
			if(i < 1){
				return 0;
			}
			else{
				return (i - 1) / 2;
			}
		}
		
		//left child of element i
		int leftChild(int i){
			return 2 * i + 1;
		}

		//right child of element i
		int rightChild(int i){
			return 2 * i + 2;
		}
		
		//time where two elements encounter one another
		double getTime(int i1, int i2){
			Node * node1 = this->maxHeap[i1];
			Node * node2 = this->maxHeap[i2];

			double value = (node2->x0 - node1->x0) / (node1->velocity - node2->velocity);

			if(node2->x0 - node1->x0 == 0 || value < 0){
				value = INT_MAX;
			} 

 			return value;
		}
		
		//return the size of the max heap
		int size(){
			return this->maxHeap.size();
		}
		
		//call getTime() function
		double intersectTime(int x1, int x2){
			return getTime(x1, x2);
		}

		//swap two elements
		void swapHeap(int i, int parent){
			std::swap(this->maxHeap[i]->index, this->maxHeap[parent]->index);
			std::swap(this->maxHeap[i], this->maxHeap[parent]);
		}

		//call swapHeap() and update the certificates
		void event(int i, int parent){	
			swapHeap(i, parent);
			int left = leftChild(i);
			int right = rightChild(i);
			int sibling = 2 * parent + (i % 2) + 1;
			int n = size();	

			int indices[] = {i, parent, sibling, left, right};

			for(int k = 0; k < 5; ++k){

				if(indices[k] > 0 && indices[k] < n){
					update(indices[k]);
				}
			}

		}
		
		//make the element go up the heap
		void swim(int i){
			this->maxHeap[i]->index = i;
			if(i && currentKey(parent(i)) < currentKey(i)){
				event(i, parent(i));
				swim(parent(i));
			}
		}
		
		//make the element go down the heap
		void sink(int i){
			int left = leftChild(i);
			int right = rightChild(i);
			int largest = i;
			if(left < size() && currentKey(left) > currentKey(i)){
				largest = left;
			}

			if(right < size() && currentKey(right) > currentKey(largest)){
				largest = right;
			}	

			this->maxHeap[i]->index = i;

			if(largest != i){
				
				event(largest, i);
				sink(largest);
			}
		}

		//compute the current key
		double currentKey(int i){
			return this->maxHeap[i]->x0 + this->maxHeap[i]->velocity * now; 
		}

	public:

		//class' constructor
		KineticHeap(){
			certificates = new MinPriorityQueue();
		}	

		//class' destructor
		~KineticHeap(){
			delete certificates;
			certificates = nullptr;
		}

		//update the certificate with the new computed deadline
		void update(int i){
			double computedTime = intersectTime(i, parent(i));
			
			if(computedTime < now || (computedTime == now && this->maxHeap[i]->velocity < this->maxHeap[parent(i)]->velocity)){
				computedTime = INT_MAX;
			}
			int position = this->node2certificate[i]->index;
			certificates->changeTime(position, computedTime);
		}

		//insert a new element and its corresponding certificate
		void insert(int id, double x0, double velocity){
			Node * node = new Node(id, x0, velocity);

			this->maxHeap.push_back(node);
			node->index = size() - 1;

			hashMap[id] = node;

			if(size() == 1){
				return;
			}

			Certificate * certificate;
			if(size() > 1){
				int childIndex = hashMap[id]->index;
				int parentIndex = parent(childIndex);

				if(currentKey(childIndex) > currentKey(parentIndex)){
					certificate = new Certificate(childIndex, now);
				}
				else{
					double computedTime = intersectTime(childIndex, parentIndex);
					certificate = new Certificate(childIndex, computedTime);		
				}
				certificates->insert(certificate);
			}
			this->node2certificate[node->index] = certificate;
			swim(node->index);
		}
		
		//remove a certificate and its corresponding element
		void remove(int i){
			int lastElementIndex = size() - 1;
			bool isGreater = currentKey(lastElementIndex) > currentKey(i);
			
			swapHeap(i, lastElementIndex);

			int id = this->maxHeap[lastElementIndex]->id;	

			delete hashMap[id];
			hashMap.erase(id);
			this->maxHeap.pop_back();

			if(isGreater){
				swim(i);
			}
			else{
				sink(i);
			}
			
			if(i == 0){
				certificates->remove(this->certificates->size() - 1);
			}
			else{
				int certificateIndex = this->node2certificate[i]->index;
				std::cout << certificateIndex << '\n';
				certificates->remove(certificateIndex);
			}
						
			int dad = parent(i);
			int left = leftChild(i);
			int right = rightChild(i);
			int sibling = 2 * dad + (i % 2) + 1;
			int n = size();	

			int indices[] = {i, dad, sibling, left, right};

			for(int k = 0; k < 5; ++k){
				if(indices[k] > 0 && indices[k] < n){
					update(indices[k]);
				}
			}

		}

		//return the top element id
		int max(){
			return this->maxHeap[0]->id;
		}

		//change the speed of the element
		void changeVelocity(int i, int velocity){
			this->maxHeap[i]->x0 = this->maxHeap[i]->x0 + (this->maxHeap[i]->velocity - velocity) * now;
			this->maxHeap[i]->velocity = velocity;

		}

		//print out the elements from the max heap 
		void print(int i, unsigned int depth){

			if(i >= size()){
				return;
			}
			
			print(2 * i + 1, depth + 3);

			for(unsigned int k = 0; k < depth; k++){
				std::cout << ' ';
			}

			std::cout << currentKey(i) << " (" << this->maxHeap[i]->x0 << ' ' << this->maxHeap[i]->velocity << ")\n";
			
			print(2 * i + 2, depth + 3);
		}

		//move the time forward
		void advance(double time){
			while(certificates->timeAt(0) <= time){
				now = certificates->timeAt(0);
				int position = certificates->positionAt(0);
				event(position, parent(position));
			}
			now = time;
		}

		//change the speed of an element given the id
		void change(int id, double velocity){
			int index = hashMap[id]->index;

			changeVelocity(index, velocity);
			int left = leftChild(index);
			int right = rightChild(index);
			int n = size();

			int indices[] = {index, left, right};
			for(int k = 0; k < 3; ++k){
				if(indices[k] > 0 && indices[k] < n){
					update(indices[k]);
				}
			}
		}	

		//return the current time
		double currentTime(){
			return this->now;
		}

		//call the remove() function to delete an element
		void removeNode(int id){
			int index = hashMap[id]->index;
			remove(index);
		}

		//call the print() function
		void printNodes(){
			print(0, 0);
		}

		//print out the certificates min heap for debugging purposes
		void printCertificates(){
			certificates->print(0, 0);
		}

		//print out the hash function for debugging purposes
		void printNode2certificate(){
			for (auto const &pair: this->node2certificate) {
        		std::cout << "{" << pair.first << ": " << pair.second->index << "}\n";
    		}
		}
};


