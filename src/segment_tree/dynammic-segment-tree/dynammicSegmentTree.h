#include "segmentTree.h"

//DynammicSegmentTree's class
class DynammicSegmentTree{

	private:
		std::vector<SegmentTree *> listOfSegmentTrees;
		std::vector<Node *> containedSegments;

		//push all the elements from second into first
		void merge(std::vector<Node *> & first, std::vector<Node *> second){
			for(unsigned int i = 0; i < second.size(); ++i){
				first.push_back(second[i]);
				second[i] = nullptr;
			}
		}

		//insert a node into the dynammic segment tree
		void insert(Node * & node){

			SegmentTree * segtree;
			unsigned int size = listOfSegmentTrees.size();
				
			//check whether list of the segment trees is empty
			if(!size){
				std::vector<Node *> intervals;
				intervals.push_back(node);
				segtree = new SegmentTree(intervals);
				segtree->build();
				listOfSegmentTrees.push_back(segtree);
			}
			else{
				std::vector<Node *> intervals;
				intervals.push_back(node);
				segtree = new SegmentTree(intervals);
				segtree->build();
				listOfSegmentTrees.push_back(segtree);

				int i =  listOfSegmentTrees.size() - 2;
				int j =  listOfSegmentTrees.size() - 1;

				//check whether two consective segment trees have the same size and merge them if so
				while(i >= 0 && (listOfSegmentTrees[i]->intervals.size() == listOfSegmentTrees[j]->intervals.size())){
					std::vector<Node *> aux = listOfSegmentTrees[j]->intervals;
					merge(aux, listOfSegmentTrees[i]->intervals);
					delete listOfSegmentTrees[i];
					delete listOfSegmentTrees[j];
					listOfSegmentTrees[i] = nullptr;
					listOfSegmentTrees[j] = nullptr;
					listOfSegmentTrees.pop_back();
					listOfSegmentTrees.pop_back();

					segtree = new SegmentTree(aux);
					segtree->build();

					listOfSegmentTrees.push_back(segtree);
					i = (listOfSegmentTrees.size()) - 2;
					j = (listOfSegmentTrees.size()) - 1; 
				}

			}
		}
	public:

		//class' default constructor
		DynammicSegmentTree(){
		}	

		//print out all segments
		void printSegments(){
			if(this->containedSegments.size() == 0){
				std::cout << "nenhum segmento" << '\n';
				return;
			}
			std::cout << this->containedSegments[0]->id;
			for(unsigned int i = 1; i < this->containedSegments.size(); ++i){
				std::cout << " " << this->containedSegments[i]->id;
			}
			std::cout << '\n';

			this->containedSegments.clear();
		}

		//call insert's function
		void insert(int start, int end, int i){
			Node * node = new Node(start, end, true, true);
			node->id = i;
			insert(node);
		}

		//segments' function
		void segments(int x){
			for(unsigned int i = 0; i < listOfSegmentTrees.size(); ++i){
				listOfSegmentTrees[i]->segments(x);
				unsigned int size = listOfSegmentTrees[i]->containedSegments.size();
				for(unsigned int j = 0; j < size; ++j){
					this->containedSegments.push_back(listOfSegmentTrees[i]->containedSegments[j]);
				}
				listOfSegmentTrees[i]->containedSegments.clear();
			}
			printSegments();
		}

		//print out the segment trees from the dynammic segment tree
		void print(){
			for(unsigned int i = 0; i < listOfSegmentTrees.size(); ++i){
				listOfSegmentTrees[i]->print();
			}
		}
};