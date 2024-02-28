
#include "treap.h"

template <typename Key, typename Item>

//stack's class
class Stack{
	private:
		Treap<Key, Item> * pushPopTreap;

	public:
		Stack(){
			pushPopTreap = new Treap<Key, Item>();
		}
		void insert(Key key, std::string operation, Item item){
			this->pushPopTreap->insertPush(key, item);
		}
		void insert(Key key, std::string operation){
			this->pushPopTreap->insertPop(key);
		}
		void remove(Key key){
			this->pushPopTreap->remove(key);
		}
		int size(Key key){
			return this->pushPopTreap->size(key);
		}
		Item top(Key key){
			return this->pushPopTreap->top(key);
		}
		Item k_th(Key key, int k){
			return this->pushPopTreap->k_th(key, k);
		}
		void print(Key key){
			this->pushPopTreap->print(key);
		}
};
