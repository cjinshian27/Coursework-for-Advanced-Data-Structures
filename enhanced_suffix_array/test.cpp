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

		unsigned int convert(std::vector<unsigned int> & vs01, 
			                 unsigned int index, 
			                 unsigned int t0Size){
			if(vs01[index] < t0Size){
				return 3 * vs01[index];				
			}
			else{
				return 3 * (vs01[index] - t0Size) + 1;
			}
		}

//==============================================================================================
		bool break2TripletsTie( int a, 
			                    int b, 
			                    int c, 
			                    int d){
			if(a < c || (a == c && b <= d)){
				return true;
			}
			else{
				return false;
			}
		}
//==============================================================================================
		bool break3TripletsTie(int a, 
			                    int b, 
			                    int c, 
			                    int d, 
			                    int e, 
			                    int f){
			if(a < d || (a == d && break2TripletsTie(b, c, e, f))){
				return true;
			}
			else{
				return false;
			}
		}




int main(){

	std::string x = "aacctaccaaccag";
	x = x + '\0';

	std::vector<unsigned int> vs01 = {10, 0, 5, 9, 1, 8, 6, 11, 7, 2, 4, 3};
	std::vector<unsigned int> vs2 = {5, 4, 3, 0, 1, 2};
	std::vector<unsigned int> rankVS01(vs01.size());
	for(unsigned int i = 0; i < vs01.size(); ++i){
		rankVS01[vs01[i]] = i;
	}

	std::cout << '\n';

	std::vector<unsigned int> text2Number = {1, 7, 6, 5, 8, 3, 8, 7, 4, 10, 2, 8, 9, 1, 2, 1, 8, 0};
	std::vector<unsigned int> rank =        {1, 3, 11, 12, 14, 3, 9, 7, 1, 3, 9, 8, 6, 13, 0};
	unsigned int textSize = text2Number.size();
	std::vector<unsigned int> vs(6);
	unsigned int t0Size = (textSize + 2) / 3;
	unsigned int t1Size = (textSize + 1) / 3;
	unsigned int t2Size = textSize / 3;
	unsigned int t01Size = t1Size + t2Size;

		unsigned int i01 = 0, i2 = 0; //próximos índices de vs0 e vs12 a serem comparados
		unsigned int k = 0; //próximo índice de vs
		bool smallestIsFrom01;
			
			while (k < textSize) {
				unsigned int value = vs01[i01];

				if (value < t0Size) {
				
					unsigned int t1 = rankVS01[value + t2Size];
					unsigned int t0 = rankVS01[vs2[i2] + 1];
					//std::cout << "from t0: " << text2Number[3 * vs01[i01]] << " " << text2Number[3 * vs2[i2] + 2] << '\n';
					//std::cout << "from t0: " <<  t1 << " " << t0 << '\n';
					smallestIsFrom01 = break2TripletsTie(text2Number[3 * vs01[i01]], 
														t1,
														text2Number[3 * vs2[i2] + 2], 
														t0);					
				}
				else {
					unsigned int t0 = rankVS01[value - t2Size + 1];
					unsigned int t1 = rankVS01[vs2[i2] + t0Size + 1];
					
					//std::cout << "from t1: " << key(text2Number, 3 * (vs01[i01] - t0Size) + 1, textSize) << " " <<  key(text2Number, 3 * vs2[i2] + 2, textSize) << '\n';
					//std::cout << "from t1: " << key(text2Number, 3 * (vs01[i01] - t0Size) + 2, textSize) << " " << key(text2Number, 3 * vs2[i2] + 3, textSize) << '\n';
					//std::cout << "from t1: " << t0 << " " << t1 << '\n';

					smallestIsFrom01 = break3TripletsTie(key(text2Number, 3 * (vs01[i01] - t0Size) + 1, textSize), 
						                                 key(text2Number, 3 * (vs01[i01] - t0Size) + 2, textSize), 
						                                 t0, 
						                                 key(text2Number, 3 * vs2[i2] + 2, textSize),
						                                 key(text2Number, 3 * vs2[i2] + 3, textSize), 
														 t1);
				}
				//std::cout << "==============================\n";
				if (smallestIsFrom01){
					vs[k++] = convert(vs01, i01++, t0Size);
					if (i01 == t01Size) {
						while (i2 < t0Size) {
							vs[k++] = 3 * vs2[i2++] + 2;
						}
					}
				}
				else {
					vs[k++] = 3 * vs2[i2++] + 2;
					if (i2 == t0Size) {
						while (i01 < t01Size) {
							vs[k++] = convert(vs01, i01++, t0Size);
						}
					}
				}

			}


			for(unsigned int i = 0; i < textSize; ++i){
				std::cout << vs[i] << " ";
			}
			std::cout << '\n';
 }