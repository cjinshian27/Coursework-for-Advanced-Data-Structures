#include "splayTree.h"
template<typename Key>

class DynamicForest{

	private:
		unsigned int id = 0;
		std::set<std::pair<Key, Key>> edgeSet;
		//std::set<SplayTree<Key> *> forest;
		std::unordered_map<Key, std::unordered_map<Key, Edge<Key> *>> mapEdges; 
		std::unordered_map<Key, SplayTree<Key> *> mapTrees; 

		//return the id of the root whose tree contains edge 
		unsigned int find(Edge<Key> * & edge){
			if(edge == nullptr){
				return 0;	
			}
			Edge<Key> * aux = edge;
			while(aux->parent != nullptr){
				aux = aux->parent;
			}
			return aux->id;
		}   

		//get the size of an edge
		unsigned int size(Edge<Key> * & edge){
			if(edge == nullptr){
				return 0;
			}
			else{
				return edge->size;
			}
		}

		//return the rank of an edge in the tree
		unsigned int order(Edge<Key> * edge){
			Edge<Key> * aux = edge;
			unsigned int k = size(edge->left) + 1;

			while(aux->parent != nullptr){
				if(aux->parent->right == aux){
					k += size(aux->parent->left) + 1;
				}
				aux = aux->parent;
			}
			return k;	
		}

		//bring the edge that contains u to the front of the euler tour
		SplayTree<Key> * bringToFront(SplayTree<Key> * splayTree, Key u){

			unsigned int index = find(getEdge(u));
			mapTrees.erase(index);
			
			if(splayTree == nullptr) return nullptr;
			
			unsigned int k = order(getEdge(u));

			std::pair<SplayTree<Key> *, SplayTree<Key> *> pairOfTrees = splayTree->split(k - 1);
			
			SplayTree<Key> * aux = pairOfTrees.second;
			aux->join(pairOfTrees.first);
	
			return aux;
		}
		
		//concatenate two splay trees
		void concatenate(SplayTree<Key> * & tree1, SplayTree<Key> * & tree2){
			if(tree1 == nullptr){
				mapTrees[tree2->root->id] = tree2;
				return;
			}

			tree1->join(tree2);
			mapTrees[tree1->root->id] = tree1;
		}

		//get the first edge that contains u 
		Edge<Key> * getEdge(Key u){
			Edge<Key> * aux;

			for(auto & x : mapEdges[u]){
				aux = x.second;
				return aux;
			}
			return nullptr;
		}

		//concatenate tree1, uv, tree2, and vu
		void concatenate(SplayTree<Key> * & tree1, Key u, SplayTree<Key> * & tree2, Key v){
			
			SplayTree<Key> * uv = new SplayTree<Key>(u, v, ++id);
			SplayTree<Key> * vu = new SplayTree<Key>(v, u, ++id);
				
			this->mapEdges[u][v] = uv->root;
			this->mapEdges[v][u] = vu->root;

			if(tree1 != nullptr){
				
				tree1->join(uv);
				tree1->join(tree2);
				tree1->join(vu);

				mapTrees[tree1->root->id] = tree1;
			}
			else{

				uv->join(tree2);
				uv->join(vu);

				mapTrees[uv->root->id] = uv;
			}
		}	

	public:	
		
		//class' constructor
		DynamicForest(){

		}		

		//check whether nodes u and v are connected
		bool isConnected(Key u, Key v){

			unsigned int id1 = find(getEdge(u));
			unsigned int id2 = find(getEdge(v));

			if(id1 == id2 && getEdge(u) != nullptr){
				return true;
			}
			return false;
		}


		//create a link between nodes u and v
		void link(Key u, Key v){

			if(isConnected(u, v)) return;
			
			if(u > v){
				std::swap(u, v);
			}
			edgeSet.insert({u, v});

			SplayTree<Key> * splayTree1 = mapTrees[find(getEdge(u))];
			SplayTree<Key> * splayTree2 = mapTrees[find(getEdge(v))];

			SplayTree<Key> * splayTree11 = bringToFront(splayTree1, u);
			SplayTree<Key> * splayTree22 = bringToFront(splayTree2, v);

			concatenate(splayTree11, u, splayTree22, v);			
		}

		//cut the edge (u, v)
		void cut(Key u, Key v){

			if(u > v){
				std::swap(u, v);
			}
			edgeSet.erase({u, v});

			unsigned int uvPosition = order(mapEdges[u][v]);
			unsigned int vuPosition = order(mapEdges[v][u]);

			unsigned int uvID = find(mapEdges[u][v]);

			SplayTree<Key> * splayTree = mapTrees[uvID];

			if(uvPosition > vuPosition){
				std::swap(uvPosition, vuPosition);
			}

			mapTrees.erase(uvID);
			
			std::pair<SplayTree<Key> *, SplayTree<Key> *> split1 = splayTree->split(uvPosition - 1);
			
			if(split1.first != nullptr){
				vuPosition -= size(split1.first->root);
			}

			std::pair<SplayTree<Key> *, SplayTree<Key> *> split2 = split1.second->split(1);
			
			std::pair<SplayTree<Key> *, SplayTree<Key> *> split3 = split2.second->split(vuPosition - 2);
			std::pair<SplayTree<Key> *, SplayTree<Key> *> split4 = split3.second->split(1);

			concatenate(split1.first, split4.second);

			if(split3.first != nullptr){
				mapTrees[split3.first->root->id] = split3.first;
			}

			mapEdges[u].erase(v);
			mapEdges[v].erase(u);

			delete(split2.first);
			delete(split4.first);
			split2.first = nullptr;
			split4.first = nullptr;		
		}
				
		//print all the edges from the forest
		void print(){
			for(auto & edge : edgeSet){
				std::cout << edge.first << " " << edge.second;
				break;
			}
			auto edge = edgeSet.begin();
			for (std::advance(edge, 1); edge != edgeSet.end(); ++edge){
			  	std::cout << "  " << edge->first << " " << edge->second;
			}
			std::cout << '\n';
		}
};			