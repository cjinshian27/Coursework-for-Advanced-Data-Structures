//suffix array's class
class SuffixArray{
	private:	
		std::string text;
		std::vector<std::pair<std::string, unsigned int>> * suffixVector;
		std::vector<unsigned int> * lcp;
		std::vector<unsigned int> * llcp;
		std::vector<unsigned int> * rlcp;
		unsigned int alphabetSize = 0;
		char lastChar = '\0';
			

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

		//count the size of the common prefix between a word and the text 
		unsigned int commomPrefix(std::string & word, unsigned int i, unsigned int j){
			unsigned int c = 0;
			unsigned int n = word.size();

			while(i + c < n && word[i + c] == text[j + i + c]){
				++c;
			}
			return c;
		}

		//use binary search to find a word in the text
		unsigned int lcpBinarySearch(std::string & word){
			unsigned int right = text.size() - 1;
			unsigned int n = word.size();
			unsigned int r = commomPrefix(word, 0, suffixVector->at(right).second);
			unsigned int c = 0, middle;
			unsigned int left = 0, l = 0;

			if(r < n && word[r] > text[suffixVector->at(right).second + r]){
				return right;
			}
			
			while(left < right - 1){
				middle = left + (right - left) / 2;
				if(l == r){
					c = commomPrefix(word, l, suffixVector->at(middle).second);
					if(l + c < n && word[l + c] > text[suffixVector->at(middle).second + l + c]){
						left = middle;
						l = l + c;
					}
					else{
						right = middle;
						r = r + c;
					}	
				}
				else if(l > r){
					if(l < llcp->at(middle)){
						left = middle;
					}	
					else if(l > llcp->at(middle)){
						right = middle;
						r = llcp->at(middle);
					}
					else{
						c = commomPrefix(word, l, suffixVector->at(middle).second);
						if(l + c < n && word[l + c] > text[suffixVector->at(middle).second + l + c]){
							left = middle;
							l = l + c;
						}
						else{
							right = middle;
							r = l + c;
						}
					}
				}
				else{
					if(r < rlcp->at(middle)){
						right = middle;
					}
					else if(r > rlcp->at(middle)){
						left = middle;
						l = rlcp->at(middle);
					}	
					else{
						c = commomPrefix(word, r, suffixVector->at(middle).second);
						if(r + c < n && word[r + c] > text[suffixVector->at(middle).second + r + c]){
							left = middle;
							l = r + c;
						}
						else{
							right = middle;
							r = r + c;
						}
					}	
				}
			}
			return left;
		}

		//print the suffix, lcp, llcp, and rlcp arrays
		void printArrays(){
			
			unsigned int n = suffixVector->size();	

			std::cout << text << '\n';
			std::cout << "VS: " << suffixVector->at(0).second;
			for(unsigned int i = 1; i < n; ++i){
				std::cout << ' ' << suffixVector->at(i).second;
			}
			std::cout << '\n';

			std::cout << "LCP: " << lcp->at(1);
			for(unsigned int i = 2; i < n; ++i){
				std::cout << ' ' << lcp->at(i);
			}
			std::cout << '\n';
		}

		//build the lcp array
		void buildLCP(){
			unsigned int i = 0, j = 0, k;
			unsigned int n = text.size();
			std::vector<unsigned int> rank(n);
			while(i < n){
				rank[suffixVector->at(i).second] = i;
				++i;
			}
			i = 0; 
			while(i < n){
				if(rank[i] > 0){
					k = suffixVector->at(rank[i] - 1).second;
					while(text[i + j] == text[j + k]){
						++j;
					}
					lcp->at(rank[i]) = j;
					if(j > 0){
						--j;
					}
				}
				++i;
			}
		}	

		//build the llcp and rlcp arrays
		void lrlcp(unsigned int i, unsigned int j, short side){
			unsigned int value = 0;
			if(i == j - 1){
				value = lcp->at(j);
			}
			else{
				unsigned int middle = i + (j - i) / 2;
				lrlcp(i, middle, 1);
				lrlcp(middle, j, 2);
				value = std::min(llcp->at(middle), rlcp->at(middle));
			}		
			//left side
			if(side == 1){
				llcp->at(j) = value;
			}
			//right side
			if(side == 2){
				rlcp->at(i) = value;
			}
		}
		
		//build the suffix array and call other functions to build other arrays
		void buildVectors(){
			text = text + '\0';
			
			unsigned int n = text.size();
			for(unsigned int i = 0; i < n; ++i){
				if((char) alphabetSize < text[i]){
					alphabetSize = text[i];
				}
			}
			++alphabetSize;
			
			suffixVector = new std::vector<std::pair<std::string, unsigned int>>(n);

			lcp = new std::vector<unsigned int>(n, 0);
			llcp = new std::vector<unsigned int>(n, 0);
			rlcp = new std::vector<unsigned int>(n, 0);

			
			for(unsigned int i = 0; i < n; ++i){
				suffixVector->at(i) = {text.substr(i, n - i), i};
			}
			
			std::sort(suffixVector->begin(), suffixVector->end());
			
			for(unsigned int i = 0; i < suffixVector->size(); ++i){
				std::cout << suffixVector->at(i).second << ' ';
			}
			std::cout << '\n';
			std::cout << "=========================================\n";

			convertText2Number();

			lastChar = 'z';// suffixVector->at(n - 1).first[0];


			//buildLCP();
			//lrlcp(0, n - 1, 0);
		}
//==============================================================================================

		void convertText2Number(){

			unsigned int n = text.size();

			std::vector<bool> inTheText(alphabetSize, 0);
			std::vector<unsigned int> rankText(alphabetSize);
			std::vector<unsigned int> text2Number(n);
			
			for(unsigned int i = 0; i < n; ++i){
				inTheText[text[i]] = 1; 
			}

			unsigned int size = alphabetSize + 1;
			unsigned int digits = 0;

			for(unsigned int i = 0; i < size; ++i){
				if(inTheText[i]){
					rankText[i] = digits;
					++digits;
				}
			}

			for(unsigned int i = 0; i < n; ++i){
				text2Number[i] = rankText[text[i]];
			}
 			
			std::vector<unsigned int> vs(text.size());
			buildSuffixVector(text2Number, vs, text.size());	
			
			debug(vs);

		}	

//==============================================================================================

		std::vector<unsigned int> buildT0T1(std::vector<unsigned int> & text2Number,
										   unsigned int t01Size,
										   unsigned int textSize){

			unsigned int i = 0, j;

			std::vector<unsigned int> triplets(textSize);
			for(i = 0; i < textSize; ++i){
				triplets[i] = i;
			}	

			std::vector<unsigned int> rank(textSize);
			std::vector<unsigned int> sortedTriplets = radixSort(text2Number, triplets, textSize);
			
			unsigned int k = 0;
			bool equalTriplets = false;
			for(unsigned int i = 0; i < textSize; ++i){
				if(i != 0 && !areTriplesEqual(text2Number, sortedTriplets[i], sortedTriplets[i - 1], textSize)){
					++k;
					if(equalTriplets){
						k = i;
						equalTriplets = false;	
					}
				}
				else if(i > 0){
					equalTriplets = true;					
				}
				rank[sortedTriplets[i]] = k;
			}

			std::vector<unsigned int> t0t1(t01Size);
			
			i = 0;
			while(3 * i < textSize){
				t0t1[i] = rank[3 * i];
				++i;
			}
			unsigned int half = i;
			i = 0;
			while(3 * i + 1 < textSize){
				t0t1[i + half] = rank[3 * i + 1];
				++i;
			}
			
			return t0t1;
		}


//==============================================================================================
		bool areTriplesEqual(std::vector<unsigned> & text2Number, 
							 unsigned int i, 
							 unsigned int j, 
							 unsigned int textSize){
			for(unsigned int k = 0; k < 3; ++k){
				if(key(text2Number, i + k, textSize) != key(text2Number, j + k, textSize)){
					return false;
				}
			}
			return true;
		}

//==============================================================================================
		std::vector<unsigned int> buildSuffixVector2(std::vector<unsigned int> & text2Number, 
								std::vector<unsigned int> & vs01,
								unsigned int t0Size,
								unsigned int t2Size,
								unsigned int textSize){

			unsigned int j = 0;
			unsigned int n = vs01.size();
			
			std::vector<unsigned int> s2;
			std::vector<unsigned int> vs2;

			for(unsigned int i = 0; i < n; ++i){
				if(vs01[i] > 0 && vs01[i] < t0Size){
					s2.push_back(3 * vs01[i] - 1);
				}
			}
			
			std::vector<unsigned int> aux(s2.size());
			countingSort(text2Number, s2, aux, textSize, 0);
			
			if(textSize % 3 == 0){
				vs2.push_back(t2Size - 1);
			}
			for(unsigned int i = 0; i < aux.size(); ++i){
				//return to original indices and subtract by 1
				vs2.push_back((aux[i] + 1) / 3 - 1);
			}

			return vs2;
		} 

//==============================================================================================
		
		void insertionSort(std::vector<unsigned int> & text2Number, std::vector<unsigned int> & v){
		    unsigned int i, aux, j;
		    for (i = 1; i < v.size(); ++i) {
		        aux = text2Number[v[i]];
		        j = i - 1;
		 
		        while (j >= 0 && text2Number[v[j]] > aux) {
		            v[j + 1] = v[j];
		            j = j - 1;
		        }
		        v[j + 1] = aux;
		    }
		}


//==============================================================================================

		void buildSuffixVector(std::vector<unsigned int> & text2Number,
			                   std::vector<unsigned int> & vs,
			                   unsigned int textSize){
			if(textSize < 3){
				//insertionSort(text2Number, vs);
				if (textSize == 1) {
					vs[0] = 0;
				}
				else{
					if (text2Number[0] < text2Number[1]) {
						vs[0] = 0;
						vs[1] = 1;
					}
					else{
						vs[0] = 1;
						vs[1] = 0;
					}
				}
				return;
			}
			unsigned int t0Size = (textSize + 2) / 3;
			unsigned int t1Size = (textSize + 1) / 3;
			unsigned int t2Size = (textSize) / 3;

			unsigned int t01Size = t0Size + t1Size;

			std::vector<unsigned int> vs01(t01Size);
			
			std::vector<unsigned int> t0t1 = buildT0T1(text2Number, t01Size, textSize);
			
			buildSuffixVector(t0t1, vs01, t01Size);	

			std::vector<unsigned int> vs2 = buildSuffixVector2(text2Number, vs01, t0Size, t2Size, textSize);			

			merge(text2Number, vs, vs01, vs2, t01Size, t0Size, t1Size, t2Size, textSize);

			return;
		}
//==============================================================================================

		void merge(std::vector<unsigned int> & text2Number,
				   std::vector<unsigned int> & vs,
				   std::vector<unsigned int> & vs01, 
				   std::vector<unsigned int> & vs2,
				   unsigned int t01Size,
				   unsigned int t0Size,
				   unsigned int t1Size,
				   unsigned int t2Size,
				   unsigned int textSize){
			
			unsigned int i01 = 0, i2 = 0; 
			unsigned int k = 0; 
			bool smallestIsFrom01;
			
			std::vector<unsigned int> rankVS01(t01Size);

			for(unsigned int i = 0; i < t01Size; ++i){
				rankVS01[vs01[i]] = i;
			}

			
			while (k < textSize) {
				int value = vs01[i01];

				if (value < t0Size) {
					
					int t1; 
					int t0; 
					int checkT1 = value + t0Size;
					int checkT0 = vs2[i2] + 1;

					if(checkT1 < t01Size){
						t1 = rankVS01[value + t0Size];
					}
					else{
						t1 = 0;
					}

					if(checkT0 < t01Size){
						t0 = rankVS01[vs2[i2] + 1];
					}
					else{
						t0 = 0;
					}
					smallestIsFrom01 = break2TripletsTie(key(text2Number, 3 * vs01[i01], textSize), 
														t1,
														key(text2Number, 3 * vs2[i2] + 2, textSize), 
														t0);										
				}
				else{
					int t1; 
					int t0;

					int checkT1 = vs2[i2] + t0Size + 1;
					int checkT0 = value - t0Size + 1;

					if(checkT1 < t01Size){
						t1 = rankVS01[vs2[i2] + t0Size + 1];
					}
					else{

						t1 = 0;
					}

					if(checkT0 < t01Size){
						t0 = rankVS01[value - t0Size + 1];
					}
					else{

						t0 = 0;
					}

					smallestIsFrom01 = break3TripletsTie(key(text2Number, 3 * (vs01[i01] - t0Size) + 1, textSize), 
						                                 key(text2Number, 3 * (vs01[i01] - t0Size) + 2, textSize), 
						                                 t0, 
						                                 key(text2Number, 3 * vs2[i2] + 2, textSize),
						                                 key(text2Number, 3 * vs2[i2] + 3, textSize), 
														 t1);
				}
				if (smallestIsFrom01){
					vs[k++] = convert(vs01, i01++, t0Size);
					if (i01 == t01Size) {
						while (i2 < t2Size) {
							vs[k++] = 3 * vs2[i2++] + 2;
						}
					}
				}
				else {
					vs[k++] = 3 * vs2[i2++] + 2;
					if (i2 == t2Size) {
						while (i01 < t01Size) {
							vs[k++] = convert(vs01, i01++, t0Size);
						}
					}
				}
			}	
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
		bool break2TripletsTie(int a, 
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

//==============================================================================================

	public:

		//suffix array's constructor
		SuffixArray(std::string & text){
			this->text = text;
			buildVectors();
		}

		//suffix array's destructor
		~SuffixArray(){
			delete llcp;
			delete rlcp;
			delete suffixVector;
			delete lcp;
			llcp = nullptr;
			rlcp = nullptr;
			suffixVector = nullptr;
			lcp = nullptr;
		}

		/*
		search for a word in the given text: 
		print out true if found, and false 
		otherwise
		*/
		void search(std::string & word){
			//word = word + '\0';
			unsigned int x = lcpBinarySearch(word);

			unsigned int n = text.size();
			
			if(x + 1 >= n){
				std::cout << "False\n"; 
				return;
			}

			std::string result = text.substr(suffixVector->at(x + 1).second, text.size());
			
			unsigned int c = 0;
			for(; c < word.size(); ++c){
				if(word[c] != result[c]){
					break;
				}
			}

			if(c == word.size()){
				std::cout << "True\n";
				return;
			}
			std::cout << "False\n";
			
		}	

		//return the indices of the occurences 
		std::vector<unsigned int> occurences(std::string & word){
			
			std::vector<unsigned int> occurencesIndices;
			unsigned int n = word.size();
			unsigned int count = 0;
			std::string modified = word;
			unsigned int i = n - 1;

			while(i >= 0){
				if(modified.at(i) != lastChar){
					modified.at(i) = (char) ((int) (word.at(i)) + 1);
					break;
				}
				else{
					++count;
				}

				if(!i) break;
				--i;
			}

			unsigned int start = lcpBinarySearch(word);
			
			if(count != n){
				unsigned int end = lcpBinarySearch(modified);
				for(i = start + 1; i <= end; ++i){
					occurencesIndices.push_back(suffixVector->at(i).second);
				}
				return occurencesIndices;
			}
			
			unsigned int upperBound = text.size() - 1;
			for(i = start + 1; i <= upperBound; ++i){
				occurencesIndices.push_back(suffixVector->at(i).second);
			}
			return occurencesIndices;
		}

		//count the number of occurances of a word in the given text
		unsigned int countOccurences(std::string & word){
			
			unsigned int n = word.size();
			unsigned int count = 0;
			std::string modified = word;
			unsigned int i = n - 1;

			while(i >= 0){
				if(modified.at(i) != lastChar){
					modified.at(i) = (char) ((int) (word.at(i)) + 1);
					break;
				}
				else{
					++count;
				}

				if(!i) break;
				--i;
			}

			unsigned int start = lcpBinarySearch(word);
			if(count != n){
				unsigned int end = lcpBinarySearch(modified);
				return end - start;
			}

			return text.size() - 1 - start;
		}

		//call printArrays()
		void print(){
			printArrays();
		}
};	