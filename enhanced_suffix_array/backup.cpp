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

		std::vector<unsigned int> buildS12(std::vector<unsigned int> & text2Number,
										   std::vector<unsigned int> & vs12,
										   std::vector<unsigned int> & s12,
										   unsigned int textSize,
										   unsigned int & countUniqueTriplets,
										   unsigned int t1Size){
			unsigned int i = 0, j;

			
			for(unsigned int k = 1; k < 3; ++k){
				j = k;
				for(;j < textSize; j = j + 3){
					s12[i++] = j;
				}
			}
			std::vector<unsigned int> aux = radixSort(text2Number, s12, textSize);
			vs12 = aux;
			std::vector<unsigned int> rank = evalRank12(text2Number, aux, countUniqueTriplets, t1Size, textSize);

			return rank;
		}
//==============================================================================================

		std::vector<unsigned int> evalRank12(std::vector<unsigned int> & text2Number, 
			 								 std::vector<unsigned int> & sortedS12,
			 								 unsigned int & countUniqueTriplets, 
			 								 unsigned int t1Size,
			 								 unsigned int textSize){
			unsigned int n = sortedS12.size();

			std::vector<unsigned int> rank(n);
			
			for(unsigned int i = 0; i < n; ++i){
				if(i && !areTriplesEqual(text2Number, sortedS12[i], sortedS12[i - 1], textSize)){
					++countUniqueTriplets;
				}
				if(sortedS12[i] % 3 == 1){
					rank[sortedS12[i] / 3] = countUniqueTriplets;
				}	
				else{
					rank[(sortedS12[i] / 3) + t1Size] = countUniqueTriplets;
				}
			}
			
			return rank;
		}

//==============================================================================================
		bool areTriplesEqual(std::vector<unsigned> & text2Number, unsigned int i, unsigned int j, unsigned int textSize){
			for(unsigned int k = 0; k < 3; ++k){
				if(key(text2Number, i + k, textSize) != key(text2Number, j + k, textSize)){
					return false;
				}
			}
			return true;
		}

//==============================================================================================
		void buildSuffixVector0(std::vector<unsigned int> & text2Number, 
				                                     std::vector<unsigned int> & vs0,
				                                     std::vector<unsigned int> & vs12, 
				                                     unsigned int textSize,
				                                     unsigned int t1Size){

			unsigned int j = 0;
			unsigned int n = vs12.size();
			unsigned int n0 = vs0.size();
			
			std::vector<unsigned int> s0(n0, 0);

			if(textSize % 3 == 1){
				s0[j++] = textSize - 1;
			}
			for(unsigned int i = 0; i < n; ++i){
				if(vs12[i] < t1Size){
					s0[j++] = 3 * vs12[i];
				}
			}
			countingSort(text2Number, s0, vs0, textSize, 0);

		} 

//==============================================================================================
		
		//insertion sort for arrays of size 3
		void insertionSort(std::vector<unsigned int> & v){
		    unsigned int i, aux, j;
		    for (i = 1; i < 3; ++i) {
		        aux = v[i];
		        j = i - 1;
		 
		        while (j >= 0 && v[j] > aux) {
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
			
			if(textSize <= 3){
				insertionSort(vs);
				return;
			}
			
			unsigned int t0Size = (textSize + 2) / 3;
			unsigned int t1Size = (textSize + 1) / 3;
			unsigned int t2Size = textSize / 3;

			unsigned int t12Size = t1Size + t2Size;
			std::vector<unsigned int> vs12(t12Size);
			std::vector<unsigned int> s12(t12Size);

			unsigned int countUniqueTriplets = 0;
			
			std::vector<unsigned int> rank = buildS12(text2Number, vs12, s12, textSize, countUniqueTriplets, t1Size);
			
			
			buildSuffixVector(rank, vs12, t12Size);	
			for(unsigned int i = 0; i < t12Size; ++i){
				vs12[rank[i]] = i;
			}
			/*
			if(countUniqueTriplets + 1 < t12Size){
				for(unsigned int i = 0; i < t12Size; ++i){
					rank[vs12[i]] = i;
				}
			}
			else{
			}
			*/
			std::vector<unsigned int> vs0(t0Size);			
			buildSuffixVector0(text2Number, vs0, vs12, textSize, t1Size);	


			int i0 = 0, i12 = 0; //próximos índices de vs0 e vs12 a serem comparados
			int k = 0; //próximo índice de vs
			auto convertT12IndexT = [=](int index) -> int {
				/*expressão lambda (uma função anônima) para
				converter um índice de vs12[] para vs*/
				if (index < t1Size) {
					//mod 1
					return(3 * index + 1);
				}
				//mod 2
				return(3 * (index - t1Size) + 2);
			};

			while (k < textSize) {
				bool smallestIsFrom0; //informa qual vetor de sufixos (0 ou 12) tem o menor elemento
				int value = vs12[i12];

				if (value < t1Size) {
					//value é sufixo mod 1
					int nextMod1Char = 3 * value + 1;
					int nextMod1Rem;
					if (nextMod1Char + 1 < textSize) {
						//o sufixo depois do caractere de mod 1 ainda não estoura o texto
						nextMod1Rem = rank[value + t1Size];
					}
					else {
						//o sufixo depois do caractere estoura o texto
						nextMod1Rem = -1;
					}
					//sufixo mod 0
					int nextMod0Char = vs0[i0];
					int nextMod0Rem;
					if (nextMod0Char + 1 < textSize) {
						//o sufixo depois do caractere de mod 0 ainda não estoura o texto
						nextMod0Rem = rank[nextMod0Char / 3];
					}
					else {
						nextMod0Rem = -1;
					}

					smallestIsFrom0 = break2TripletsTie(text2Number[nextMod0Char], 
														nextMod0Rem, 
														text2Number[nextMod1Char], 
														nextMod1Rem);
				}
				else {
					//value é sufixo mod 2
					int firstMod2Char = 3 * (value - t1Size) + 2;
					
					int nextMod2Rem;
					if (firstMod2Char + 2 < textSize) {
						//o sufixo ainda não estoura o texto
						nextMod2Rem = rank[value - t1Size + 1];
					}
					else {
						//o sufixo estourou o texto
						nextMod2Rem = -1;
					}
					//sufixo mod 0
					int firstMod0Char = vs0[i0];
					int nextMod0Rem;
					if (firstMod0Char + 2 < textSize) {
						//ainda não estoura o texto
						nextMod0Rem = rank[firstMod0Char / 3 + t1Size];
					}
					else {
						nextMod0Rem = -1;
					}


					smallestIsFrom0 = break3TripletsTie(text2Number[firstMod0Char], 
						                                key(text2Number, 1 + firstMod0Char, textSize), 
						                                nextMod0Rem, 
						                                text2Number[firstMod2Char], 
						                                key(text2Number, 1 + firstMod2Char, textSize), 
						                                nextMod2Rem);
				}
				if (smallestIsFrom0){
					vs[k++] = vs0[i0++];
					if (i0 == t0Size) {
						//acabou o vetor vs0, vamos copiar o resto de vs12
						while (i12 < t12Size) {
							vs[k++] = convertT12IndexT(vs12[i12++]);
						}
					}
				}
				else {
					//devemos copiar um elemento de vs12
					vs[k++] = convertT12IndexT(vs12[i12++]);
					if (i12 == t12Size) {
						//acabou vs12, termina de copiar vs0
						while (i0 < t0Size) {
							vs[k++] = vs0[i0++];
						}
					}
				}

			}
		
			return;
		}
//==============================================================================================

		void print2(unsigned int a, unsigned int b, unsigned int c, unsigned int d){
			std::cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
		}
//==============================================================================================
		void print3(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e, unsigned int f){
			std::cout << a << ' ' << b << ' ' << c << ' ' << d << ' ' <<  e << ' ' << f << '\n';
		}

//==============================================================================================
		bool break2TripletsTie(int a, int b, int c, int d){
			if(a < c || (a == c && b <= d)){
				return true;
			}
			else{
				return false;
			}
		}
//==============================================================================================
		bool break3TripletsTie(int a, int b,  int c,  int d,  int e,  int f){
			if(a < d || (a == d && break2TripletsTie(b, c, e, f))){
				return true;
			}
			else{
				return false;
			}
		}

//==============================================================================================
		unsigned int convertKeys(std::vector<unsigned int> & vs12, unsigned int n0, unsigned int index){
			if(vs12[index] < n0){
				return 3 * vs12[index] + 1;
			}
			else{
				return 3 * (vs12[index] - n0) + 2; 
			}
		}


//==============================================================================================

		std::vector<unsigned int> merge(std::vector<unsigned int> & text2Number,
				   						std::vector<unsigned int> & vs0, 
				   						std::vector<unsigned int> & vs12,
				   						std::vector<unsigned int> & rank,
				   						unsigned int textSize,
				   						unsigned int t0size,
				   						unsigned int t1size){
			
			unsigned int t12size = vs12.size();

			std::vector<unsigned int> finalSV(text.size());
			/*
			for(unsigned int i = 0; i < n; ++i){
				if(vs12[i] < n0){
					vs12[i] = 3 * vs12[i] + 1;
				}
				else{
					vs12[i] = 3 * (vs12[i] - n0) + 2; 
				}
			}
			*/

			unsigned int i0 = 0, i12 = 0, k = 0;

			while(k < textSize){
				bool smallest;
				unsigned int value = vs12[i12];
				unsigned int nextModeRem1;
				if(value < t0size){
					unsigned int nextMod1Char = 3 * value + 1;
					unsigned int nextMod1Rem;	
					
					if(nextMod1Char + 1 < textSize){
						nextMod1Char = rank[value + t1size];
					}
					else{
						nextMod1Rem = -1;
					}
					unsigned int nextMod0Char = vs0[i0];
					
					unsigned int nextMod0Rem;
					if(nextMod0Char + 1 < textSize){
						nextMod0Rem = rank[nextMod0Char / 3];
					}
					else{	
						nextMod0Rem = -1;
					}
					smallest = break2TripletsTie(text2Number[nextMod0Char], 
						                         nextMod0Rem, 
						                         text2Number[nextMod1Char], 
						                         nextMod1Rem);
				}
				else{
					unsigned int firstMod2Char = 3 * (value - t0size) + 2;
					unsigned int nextMod2Rem;
					if(firstMod2Char + 2 < textSize){
						nextMod2Rem = rank[value - t1size + 1];
					}
					else{
						nextMod2Rem = -1;
					}

					unsigned int firstMod0Char = vs0[i0];
					unsigned int nextMod0Rem;

					if(firstMod0Char + 2 < textSize){
						nextMod0Rem = rank[firstMod0Char / 3 + t1size];
					}
					else{
						nextMod0Rem = -1;
					}
					smallest = break3TripletsTie(text2Number[firstMod0Char], 
						                         key(text2Number, 1 + firstMod0Char, textSize), 
						                         nextMod0Rem, 
						                         text2Number[firstMod2Char], 
						                         key(text2Number, 1 + firstMod2Char, textSize), 
						                         nextMod2Rem);
				}
				if(smallest) {
					finalSV[k++] = vs0[i0++];
					if(i0 == t0size){
						while (i12 < t12size) {
							finalSV[k++] = convertKeys(vs12, i12++, t1size);
						}
					}
				}
				else{
					finalSV[k++] = convertKeys(vs12, i12++, t1size);
					if (i12 == t12size) {
						while (i0 < t0size) {
							finalSV[k++] = vs0[i0++];
						}
					}
				}
			}
			return finalSV;
		}
//==============================================================================================
		std::vector<unsigned int> radixSort(std::vector<unsigned int> & text2Number,
				       						std::vector<unsigned int> & s12,
				       						unsigned int textSize){

			std::vector<unsigned int> sortedS12(s12.size(), 0);

			countingSort(text2Number, s12, sortedS12, textSize, 2);
			countingSort(text2Number, sortedS12, s12, textSize, 1);
			countingSort(text2Number, s12, sortedS12, textSize, 0);

			return sortedS12;
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