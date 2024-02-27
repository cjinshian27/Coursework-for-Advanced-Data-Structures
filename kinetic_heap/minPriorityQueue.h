
//minimum priority queue's class 
class MinPriorityQueue{
	private:
		std::vector<Certificate *> minHeap;
	public:
		//parent of certificate i
		int parent(int i){
			if(i < 1){
				return 0;
			}
			else{
				return (i - 1) / 2;
			}
		}

		//left child of certificate i
		int leftChild(int i){
			return 2 * i + 1;
		}

		//right child of certificate i
		int rightChild(int i){
			return 2 * i + 2;
		}

		//size of certificate i
		int size(){
			return this->minHeap.size();
		}

		//swap two certificates
		void swapCertificate(int i, int parent){
			std::swap(this->minHeap[i]->index, this->minHeap[parent]->index);
			std::swap(this->minHeap[i], this->minHeap[parent]);
		}

		//make the certificate go down the heap
		void sink(int i){
			int left = leftChild(i);
			int right = rightChild(i);
			int smallest = i;
			if(left < size() && this->minHeap[left]->time < this->minHeap[i]->time){
				smallest = left;
			}

			if(right < size() && this->minHeap[right]->time < this->minHeap[smallest]->time){
				smallest = right;
			}	

			this->minHeap[i]->index = i;
			if(smallest != i){
				swapCertificate(i, smallest);
				sink(smallest);
			}
		}

		//make the certificate go up the heap
		void swim(int i){
			this->minHeap[i]->index = i;

			if(i && this->minHeap[parent(i)]->time > this->minHeap[i]->time){
				
				swapCertificate(i, parent(i));

				swim(parent(i));
			}
		}

		//insert a new certificate
		void insert(Certificate * & certificate){
			this->minHeap.push_back(certificate);
			int index = size() - 1;
			swim(index);
		}

		//remove the certificate i 
		void remove(int i){
			int lastCertificate = size() - 1;
 			bool isGreater = this->minHeap[lastCertificate]->time > this->minHeap[i]->time;

			swapCertificate(i, lastCertificate);
			std::swap(this->minHeap[i]->position, this->minHeap[lastCertificate]->position);

			this->minHeap.pop_back();
			
			if(isGreater){
				sink(i);
			}
			else{
				swim(i);
			}
		}

		//return the time of certificate i
		double timeAt(int i){
			return this->minHeap[i]->time;
		}	

		//change time of certificate i to a new one
		void changeTime(int i, double time){
			if(minHeap[i]->time > time){
				this->minHeap[i]->time = time;
				swim(i);	
			}
			else{
				this->minHeap[i]->time = time;
				sink(i);
			}
		}

		//return the position of certificate i
		int positionAt(int i){
			return this->minHeap[i]->position;
		}

		//print out the certificates from the heap
		void print(int i, unsigned int depth){
			if(i >= size()){
				return;
			}
			print(2 * i + 1, depth + 3);

			for(unsigned int k = 0; k < depth; k++){
				std::cout << ' ';
			}
			std::cout << "(" << this->minHeap[i]->position << ' ';
			if(this->minHeap[i]->time >= 2.14748e+09){
				std::cout << "ထ ";
			}
			else{
				std::cout << this->minHeap[i]->time;
			}
			std::cout << ")\n";
			
			print(2 * i + 2, depth + 3);
		}
};