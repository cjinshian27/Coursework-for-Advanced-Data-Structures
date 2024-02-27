#include <vector>
#include <iostream>

//==============================================================================================

		unsigned int key(std::vector<unsigned int> & text2Number, int index, int textSize){
			if(index < 0 || index >= textSize){
				return 0;
			}
			return text2Number[index];
		}
//==============================================================================================

		void countingSort(std::vector<unsigned int> & text2Number,
						  std::vector<unsigned int> & origin,
						  std::vector<unsigned int> & destiny,
		                  unsigned int textSize,
		                  unsigned int delta){
			
			unsigned int alphabetSize = 300;
			std::vector<unsigned int> frequency(alphabetSize, 0);
			unsigned int j, count;
			unsigned int sum = 0;
			unsigned int n = origin.size();
			
			for(unsigned int i = 0; i < n; ++i){
				j = key(text2Number, origin[i] + delta, textSize);
				frequency[j] = frequency[j] + 1;
			}
			for(unsigned int i = 0; i < alphabetSize; ++i){
				count = frequency[i];
				frequency[i] = sum;
				sum = sum + count;
			}
			for(unsigned int i = 0; i < n; ++i){
				j = key(text2Number, origin[i] + delta, textSize);
				destiny[frequency[j]] = origin[i];
				frequency[j] = frequency[j] + 1;
			}
		}
		void debug(std::vector<unsigned int> & v){
			unsigned int n = v.size();
			if(!n){
				return;
			}
			std::cout << v[0];
			for(unsigned int i = 1; i < n; ++i){
				std::cout << ' ' << v[i];			
			}
			std::cout << '\n';
		}
//==============================================================================================
		std::vector<unsigned int> radixSort(std::vector<unsigned int> & text2Number,
				       						std::vector<unsigned int> & s01,
				       						unsigned int textSize){

			std::vector<unsigned int> vs01(s01.size(), 0);

			countingSort(text2Number, s01, vs01, textSize, 2);
			countingSort(text2Number, vs01, s01, textSize, 1);
			countingSort(text2Number, s01, vs01, textSize, 0);

			return vs01;
		}


		std::vector<unsigned int> buildSuffixVector2(std::vector<unsigned int> & text2Number, 
								std::vector<unsigned int> & vs01,
								unsigned int t0Size,
								unsigned int textSize){

			unsigned int j = 0;
			unsigned int n = vs01.size();
			std::vector<unsigned int> s0(t0Size, 0);
			std::vector<unsigned int> vs0(t0Size);

			if(textSize % 3 == 1){
				s0[j++] = textSize - 1;
			}
			for(unsigned int i = 0; i < n; ++i){
				if(vs01[i] < t0Size){
					s0[j++] = 3 * vs01[i] + 2;
				}
			}

			countingSort(text2Number, s0, vs0, textSize, 2);
			
			for(unsigned int i = 0; i < t0Size; ++i){
				vs0[i] = (vs0[i] - 2) / 3;
			}
			debug(vs0);
			return vs0;
		}



int main(){
	std::vector<unsigned int> text2Number = {1, 7, 6, 5, 8, 3, 8, 7, 4, 10, 2, 8, 9, 1, 2, 1, 8, 0};
	std::vector<unsigned int> vs01 = {10, 0, 5, 9, 1, 8, 6, 11, 7, 2, 4, 3};
	
	unsigned int textSize = text2Number.size();
	unsigned int t0Size = (textSize + 2) / 3;
	unsigned int t1Size = (textSize + 1) / 3;
	unsigned int t2Size = textSize / 3;
	unsigned int t01Size = t0Size + t1Size;

	std::vector<unsigned int> s01(t01Size);

	std::vector<unsigned int> s2;
	std::vector<unsigned int> vs2;

	for(unsigned int i = 0; i < vs01.size(); ++i){
		if(vs01[i] > vs01[i] < t0Size){
			s2.push_back(3 * (vs01[i]) + 2);
		}
	}

	std::vector<unsigned int> aux(s2.size());
	countingSort(text2Number, s2, aux, textSize, 0);
	
	
	if(textSize % 3 == 0){
		vs2.push_back(t2Size - 1);
	}
	
	for(unsigned int i = 0; i < aux.size(); ++i){
		//return to original indices and subtract by 1
		vs2.push_back((aux[i] - 2) / 3);
	}
	debug(vs2);	
}	