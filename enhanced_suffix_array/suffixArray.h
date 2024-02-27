//suffix array's class
class SuffixArray{
	private:	
		std::string text;
		std::vector<unsigned int> suffixVector;
		std::vector<unsigned int> * lcp;
		std::vector<unsigned int> * llcp;
		std::vector<unsigned int> * rlcp;
		unsigned int alphabetSize = 0;
		char lastChar = '\0';
			
		//print out a vector for debugging purposes
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
			unsigned int r = commomPrefix(word, 0, suffixVector.at(right));
			unsigned int c = 0, middle;
			unsigned int left = 0, l = 0;

			if(r < n && word[r] > text[suffixVector.at(right) + r]){
				return right;
			}
			
			while(left < right - 1){
				middle = left + (right - left) / 2;
				if(l == r){
					c = commomPrefix(word, l, suffixVector.at(middle));
					if(l + c < n && word[l + c] > text[suffixVector.at(middle) + l + c]){
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
						c = commomPrefix(word, l, suffixVector.at(middle));
						if(l + c < n && word[l + c] > text[suffixVector.at(middle) + l + c]){
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
						c = commomPrefix(word, r, suffixVector.at(middle));
						if(r + c < n && word[r + c] > text[suffixVector.at(middle) + r + c]){
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

		//print the suffix and lcp arrays
		void printArrays(){
			
			unsigned int n = suffixVector.size();	

			std::cout << text << '\n';
			std::cout << "VS: " << suffixVector.at(0);
			for(unsigned int i = 1; i < n; ++i){
				std::cout << ' ' << suffixVector.at(i);
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
				rank[suffixVector.at(i)] = i;
				++i;
			}
			i = 0; 
			while(i < n){
				if(rank[i] > 0){
					k = suffixVector.at(rank[i] - 1);
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
			
			lcp = new std::vector<unsigned int>(n, 0);
			llcp = new std::vector<unsigned int>(n, 0);
			rlcp = new std::vector<unsigned int>(n, 0);
			
			convertText2Number();

			lastChar = text[suffixVector.at(n - 1)];
			
			buildLCP();
			lrlcp(0, n - 1, 0);
		}

		//convert text to numeric text, assigning each letter to a number
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
					rankText[i] = digits++;
				}
			}

			for(unsigned int i = 0; i < n; ++i){
				text2Number[i] = rankText[text[i]];
			}
 			
			std::vector<unsigned int> sa(text.size());
			buildSuffixArray(text2Number, sa, text.size());	
			suffixVector = sa;
		}	

		//build t0t1 from given numeric text
		std::vector<unsigned int> buildT0T1(std::vector<unsigned int> & text2Number,
										    unsigned int t01Size,
										    unsigned int textSize){

			unsigned int i = 0;

			std::vector<unsigned int> triplets(textSize);
			for(i = 0; i < textSize; ++i){
				triplets[i] = i;
			}	

			std::vector<unsigned int> rank(textSize);
			std::vector<unsigned int> sortedTriplets = radixSort(text2Number, triplets, textSize);
			
			unsigned int k = 0;
			bool equalTriplets = false;

			//build vector rank from the sorted triplets
			for(unsigned int i = 0; i < textSize; ++i){
				if(i != 0 && !areTripletsEqual(text2Number, sortedTriplets[i], sortedTriplets[i - 1], textSize)){
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

		//check whether two triplets are the same
		bool areTripletsEqual(std::vector<unsigned> & text2Number, 
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

		//build suffix array 2 from suffix array 01
		std::vector<unsigned int> buildSuffixArray2(std::vector<unsigned int> & text2Number, 
													std::vector<unsigned int> & sa01,
													unsigned int t0Size,
													unsigned int t2Size,
													unsigned int textSize){

			unsigned int n = sa01.size();
			
			std::vector<unsigned int> s2;
			std::vector<unsigned int> sa2;

			if(textSize % 3 == 0){
				if(text2Number[3 * (t2Size - 1) + 2] == 0){
					sa2.push_back(t2Size - 1);
					
					for(unsigned int i = 0; i < n; ++i){
						if(sa01[i] > 0 && sa01[i] < t0Size){
							s2.push_back(3 * (sa01[i]) - 1);
						}
					}
					std::vector<unsigned int> aux(s2.size());
					countingSort(text2Number, s2, aux, textSize, 0);
					for(unsigned int i = 0; i < aux.size(); ++i){
						//return to original indices and subtract by 1
						sa2.push_back((aux[i] + 1) / 3 - 1);
					}
				}
				else{
					for(unsigned int i = 0; i < n; ++i){
						if(sa01[i] < t0Size){
							s2.push_back(3 * (sa01[i]) + 2);
						}
					}
					std::vector<unsigned int> aux(s2.size());
	
					countingSort(text2Number, s2, aux, textSize, 0);
	
					for(unsigned int i = 0; i < aux.size(); ++i){
						sa2.push_back((aux[i] - 2) / 3);	
					}
				}
			}
			else{					
				for(unsigned int i = 0; i < n; ++i){
					if(sa01[i] > 0 && sa01[i] < t0Size){
						s2.push_back(3 * (sa01[i]) - 1);
					}
				}
				std::vector<unsigned int> aux(s2.size());
				countingSort(text2Number, s2, aux, textSize, 0);
				for(unsigned int i = 0; i < aux.size(); ++i){
					//return to original indices and subtract by 1
					sa2.push_back((aux[i] + 1) / 3 - 1);
				}
			}
			return sa2;
		} 

		//build the suffix array recursively
		void buildSuffixArray(std::vector<unsigned int> & text2Number,
			                  std::vector<unsigned int> & sa,
			                  unsigned int textSize){
			
			if(textSize < 3){
				if (textSize == 1) {
					sa[0] = 0;
				}
				else{
					if (text2Number[0] < text2Number[1]) {
						sa[0] = 0;
						sa[1] = 1;
					}
					else{
						sa[0] = 1;
						sa[1] = 0;
					}
				}
				return;
			}

			unsigned int t0Size = (textSize + 2) / 3;
			unsigned int t1Size = (textSize + 1) / 3;
			unsigned int t2Size = (textSize) / 3;

			unsigned int t01Size = t0Size + t1Size;

			std::vector<unsigned int> sa01(t01Size);
			
			std::vector<unsigned int> t0t1 = buildT0T1(text2Number, t01Size, textSize);
				
			buildSuffixArray(t0t1, sa01, t01Size);	

			std::vector<unsigned int> sa2 = buildSuffixArray2(text2Number, sa01, t0Size, t2Size, textSize);			

			merge(text2Number, sa, sa01, sa2, t01Size, t0Size, t2Size, textSize);

			return;
		}

		//get the rank value
		unsigned int getRankValue(std::vector<unsigned int> & rank, unsigned int index, unsigned int t01Size){
			if(index < t01Size){
				return rank[index];
			}
			else{
				return 0;
			}
		}

		//merge suffix array 01 with suffix array 2
		void merge(std::vector<unsigned int> & text2Number,
				   std::vector<unsigned int> & sa,
				   std::vector<unsigned int> & sa01, 
				   std::vector<unsigned int> & sa2,
				   unsigned int t01Size,
				   unsigned int t0Size,
				   unsigned int t2Size,
				   unsigned int textSize){
			
			unsigned int i01 = 0, i2 = 0; 
			unsigned int k = 0; 
			bool smallestIsFrom01;
			
			std::vector<unsigned int> rankSA01(t01Size);

			for(unsigned int i = 0; i < t01Size; ++i){
				rankSA01[sa01[i]] = i;
			}

			while (k < textSize) {
				unsigned int value = sa01[i01];

				if (value < t0Size) {
					
					unsigned int t1 = getRankValue(rankSA01, value + t0Size, t01Size); 
					unsigned int t0 = getRankValue(rankSA01, sa2[i2] + 1, t01Size); 
					
					smallestIsFrom01 = break2TripletsTie(key(text2Number, 3 * sa01[i01], textSize), 
														t1,
														key(text2Number, 3 * sa2[i2] + 2, textSize), 
														t0);										
				}
				else{
					unsigned int t1 = getRankValue(rankSA01, sa2[i2] + t0Size + 1, t01Size); 
					unsigned int t0 = getRankValue(rankSA01, value - t0Size + 1, t01Size);

					smallestIsFrom01 = break3TripletsTie(key(text2Number, 3 * (sa01[i01] - t0Size) + 1, textSize), 
						                                 key(text2Number, 3 * (sa01[i01] - t0Size) + 2, textSize), 
						                                 t0, 
						                                 key(text2Number, 3 * sa2[i2] + 2, textSize),
						                                 key(text2Number, 3 * sa2[i2] + 3, textSize), 
														 t1);
				}
				if (smallestIsFrom01){
					sa[k++] = convertIndex(sa01, i01++, t0Size);
					if (i01 == t01Size) {
						while (i2 < t2Size) {
							sa[k++] = 3 * sa2[i2++] + 2;
						}
					}
				}
				else {
					sa[k++] = 3 * sa2[i2++] + 2;
					if (i2 == t2Size) {
						while (i01 < t01Size) {
							sa[k++] = convertIndex(sa01, i01++, t0Size);
						}
					}
				}
			}	
		}

		//convert suffix array 01 indices to suffix array indices
		unsigned int convertIndex(std::vector<unsigned int> & sa01, 
			                 unsigned int index, 
			                 unsigned int t0Size){
			if(sa01[index] < t0Size){
				return 3 * sa01[index];				
			}
			else{
				return 3 * (sa01[index] - t0Size) + 1;
			}
		}

		//break a tie between two triplets
		bool break2TripletsTie(unsigned int a, 
			                   unsigned int b, 
			                   unsigned int c, 
			                   unsigned int d){
			if(a < c || (a == c && b <= d)){
				return true;
			}
			else{
				return false;
			}
		}

		//break a tie between three triplets
		bool break3TripletsTie(unsigned int a, 
			                   unsigned int b, 
			                   unsigned int c, 
			                   unsigned int d, 
			                   unsigned int e, 
			                   unsigned int f){
			if(a < d || (a == d && break2TripletsTie(b, c, e, f))){
				return true;
			}
			else{
				return false;
			}
		}

		//radix sort the suffix array 01
		std::vector<unsigned int> radixSort(std::vector<unsigned int> & text2Number,
				       						std::vector<unsigned int> & s01,
				       						unsigned int textSize){

			std::vector<unsigned int> sa01(s01.size(), 0);

			countingSort(text2Number, s01, sa01, textSize, 2);
			countingSort(text2Number, sa01, s01, textSize, 1);
			countingSort(text2Number, s01, sa01, textSize, 0);

			return sa01;
		}

		//get the numeric text value given the index
		unsigned int key(std::vector<unsigned int> & text2Number, unsigned int index, unsigned int textSize){
			if(index < textSize){
				return text2Number[index];
			}
			return 0;
		}

		//counting sort
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
			delete lcp;
			llcp = nullptr;
			rlcp = nullptr;
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

			std::string result = text.substr(suffixVector.at(x + 1), text.size());
			
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
					occurencesIndices.push_back(suffixVector.at(i));
				}
				return occurencesIndices;
			}
			
			unsigned int upperBound = text.size() - 1;
			for(i = start + 1; i <= upperBound; ++i){
				occurencesIndices.push_back(suffixVector.at(i));
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