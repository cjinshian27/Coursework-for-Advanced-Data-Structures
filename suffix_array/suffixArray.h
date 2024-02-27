//suffix array's class
class SuffixArray{
	private:	
		std::string text;
		std::vector<std::pair<std::string, unsigned int>> * suffixVector;
		std::vector<unsigned int> * lcp;
		std::vector<unsigned int> * llcp;
		std::vector<unsigned int> * rlcp;
		char lastChar = '\0';

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
			
			suffixVector = new std::vector<std::pair<std::string, unsigned int>>(n);

			lcp = new std::vector<unsigned int>(n, 0);
			llcp = new std::vector<unsigned int>(n, 0);
			rlcp = new std::vector<unsigned int>(n, 0);

			for(unsigned int i = 0; i < n; ++i){
				suffixVector->at(i) = {text.substr(i, n - i), i};
			}
			
			std::sort(suffixVector->begin(), suffixVector->end());
			lastChar = suffixVector->at(n - 1).first[0];
			buildLCP();
			lrlcp(0, n - 1, 0);
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

			std::string result = suffixVector->at(x + 1).first;
			
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