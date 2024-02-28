#include "newnode.h"

/*********************************************************************************

	          						CLASS DEQUE

**********************************************************************************/


template<typename Item> 

//generic deque's class
class Deque{
	private:
		NewNode<Item> * root;
		NewNode<Item> * first;
		NewNode<Item> * last;
		
		//swap the deque's ends
		Deque * swap(){
			return new Deque(this->last, this->first);
		}

		//return the k-th level ancestor of a node
		inline NewNode<Item> * levelAncestor(unsigned int k, NewNode<Item> * u){
			
			unsigned int y = 0;
			
			y = u->depth - k;
			
			while(u->depth != y){
				if(u->jump->depth >= y){
					u = u->jump;
				}
				else{
					u = u->parent;
				}
			}
			return u;
		}

		//compute the jump for each node
		inline void addLeaf(NewNode<Item> * u){
			
			NewNode<Item> * v = u->parent;

			if(v->jump->depth != 0 && v->depth - v->jump->depth == v->jump->depth - v->jump->jump->depth){
				u->jump = v->jump->jump;
			}
			else{
				u->jump = v;
			}
		}

		//return the lowest common ancestor between two nodes
		inline NewNode<Item> * lowestCommonAncestor(NewNode<Item> * u, NewNode<Item> * v){
			if(u->depth > v->depth){
				NewNode<Item> * w = u;
				u = v;
				v = w;
			}

			v = levelAncestor(v->depth - u->depth, v);
			
			if(u == v){
				return u;
			}
			
			while(u->parent != v->parent){
				if(u->jump != v->jump){
					u = u->jump;
					v = v->jump;
				}
				else{
					u = u->parent;
					v = v->parent;
				}	
			}
			return u->parent;
		}
	public:	

	//deque's default constructor
	Deque(){
		root = new NewNode<Item>();
		this->first = root;
		this->last = root;
	}

	//deque's parameterized constructor
	Deque(NewNode<Item> * first, NewNode<Item> * last){
		this->first = first;
		this->last = last;
		addLeaf(this->first);
		addLeaf(this->last);

	}

	//push the element into the front of the deque
	inline Deque * pushFront(Item x){
		Deque * d;	
		if(this->first->depth == 0){
			NewNode<Item> * u = new NewNode<Item>(x, this->first, 1);
			d = new Deque(u, u);
		}
		else{
			NewNode<Item> * k = new NewNode<Item>(x, this->first, this->first->depth + 1);
			d = new Deque(k, this->last);
		}
		return d;
	}	

	//push the element into the back of the deque
	inline Deque * pushBack(Item x){
		return this->swap()->pushFront(x)->swap();
	}

	//return the deque's front element
	inline Item front(){
		return this->first->val;
	}	

	//return the deque's last element
	inline Item back(){
		return this->last->val;
	}

	//pop the front element out of the deque
	inline Deque * popFront(){
		NewNode<Item> * mid = lowestCommonAncestor(this->first, this->last);
		if(this->first != mid){
			return new Deque(this->first->parent, this->last);
		}
		
		NewNode<Item> * ancestor = levelAncestor(this->last->depth - this->first->depth - 1, this->last);
		return new Deque(ancestor, this->last);
	}

	//pop the last element out of the deque
	inline Deque * popBack(){
		return this->swap()->popFront()->swap();
	}

	//return the k-th element from the deque
	inline Item k_th(unsigned int k){
		NewNode<Item> * mid = lowestCommonAncestor(this->first, this->last);
		unsigned int length1 = abs(mid->depth - this->first->depth);
		 
		if(k - 1 <= length1){
			NewNode<Item> * kth = levelAncestor(k - 1, this->first);
			return kth->val;
		}
		
		unsigned int length2 = abs(mid->depth - this->last->depth);

		NewNode<Item> * kth = levelAncestor(length2 - (k - 1 - length1), this->last);
		return kth->val;
	}

	//print out the deque's elements from front to back
	inline void print(){

		NewNode<Item> * mid = lowestCommonAncestor(this->first, this->last);

		NewNode<Item> * x = this->first;

		while(x->depth != mid->depth){
			std::cout << x->val << ' ';
			x = x->parent;
		}

		std::cout << x->val;

		x = this->last;

		std::stack<Item> stackOfNodes; 
		
		while(x->depth != mid->depth){
			stackOfNodes.push(x->val);
			x = x->parent;
		}

		while(!stackOfNodes.empty()){
			std::cout << ' ' << stackOfNodes.top();
			stackOfNodes.pop();
		}
		std::cout << '\n';
	}
};
