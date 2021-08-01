#include <deque>
#include <fstream>
#include <string>
#include <stdio.h>

struct wordFreq{
	std::string word = "";
	int frequency = 0;
};

int find_substring_instance(std::string str, std::string substring, int instance);
int find_wordDeque_index(std::deque<struct wordFreq> wordDeque, std::string word);
std::deque<int> find_substring_positions(std::string str, std::string search, std::deque<char> add);
std::deque<std::string> find_context_for_word(std::string word, std::string sourceFile);
std::deque<std::string> find_references_for_word(std::string word, std::string sourceFile);
std::deque<std::string> parse_line(std::string line);
std::deque<struct wordFreq> sort_deque(std::deque<struct wordFreq> dqe);
std::string duplicate_string(std::string str, int times);
std::string make_lowercase_string(std::string str);
std::string make_uppercase_string(std::string str);
std::string sanitize_string(std::string str);

int main(int argc, char *argv[]){
	if(argc != 4){
		printf("Too many or too few arguments, exiting.\n"); return 1;
	}
	std::string sourceFile = argv[1];
	int min = std::atoi(argv[2]); int max = std::atoi(argv[3]);
	
	std::ifstream readFile; readFile.open(sourceFile); std::string line;
	std::deque<std::string> parsedLine;
	std::deque<struct wordFreq> wordDeque; int wordDequeIndex;
	
	while(getline(readFile, line)){
		parsedLine = parse_line(sanitize_string(line.substr(line.find("|"), line.size() - 1)));
		for(std::string &i : parsedLine){
			wordDequeIndex = find_wordDeque_index(wordDeque, i);
			if(wordDequeIndex == -1){
				wordDeque.push_back({i, 1});
			}else{
				wordDeque[wordDequeIndex].frequency++;
			}
		}
	}
	
	wordDeque = sort_deque(wordDeque);
	std::deque<std::string> entries;
	for(struct wordFreq &i : wordDeque){
		if(i.frequency >= min && i.frequency <= max){
			printf("%s (%i)\n", i.word.c_str(), i.frequency);
			entries = find_context_for_word(i.word, sourceFile);
			for(std::string &j : entries){
				printf("\t%s\n", j.c_str());
			}
		}
	}
	
	return 0;
}

int find_substring_instance(std::string str, std::string substring, int instance){
	int counter = 0; int place; int ret = 0;
	while(counter < instance){
		if(str.find(substring) == std::string::npos){
			return -1;
		}
		place = str.find(substring);
		ret += place + substring.size();
		str.erase(0, place + substring.size());
		counter++;
	}
	//printf("%s\n", str.c_str());
	return ret - substring.size();
}

int find_wordDeque_index(std::deque<struct wordFreq> wordDeque, std::string word){
	int index = 0;
	for(struct wordFreq &i : wordDeque){
		if(i.word == word){
			return index;
		}
		index++;
	}
	return -1;
}

std::deque<std::string> find_context_for_word(std::string word, std::string sourceFile){
	std::deque<std::string> ret; std::deque<std::string> refs = find_references_for_word(word, sourceFile); int refsIndex = 0;
	std::string verseText; int wordIndex, startContext, endContext, contextFactor; contextFactor = 60;
	std::deque<int> wordIndicies;
	std::ifstream readFile; readFile.open(sourceFile); std::string line = "|";
	while(getline(readFile, line)){
		if(refs[refsIndex] == line.substr(0, line.find("|"))){
			verseText = " " + line.substr(line.find("|") + 1, line.size() - 1);
			wordIndicies = find_substring_positions(make_lowercase_string(verseText), word, {';', ':', '\'', '-', '.', ',', ' '});
			for(int i = 0; i < wordIndicies.size(); i++){
				verseText.insert(wordIndicies[i] + i + i, "*");
				verseText.insert(wordIndicies[i] + word.size() + i + i + 1, "*");
				wordIndex = wordIndicies[i] + i;
				if(wordIndex <= contextFactor){
					startContext = 0;
				}else{
					startContext = wordIndex - contextFactor + verseText.substr(wordIndex - contextFactor, wordIndex).find(" ") + 1;
				}
				endContext = wordIndex + word.size() + contextFactor;
				if(endContext >= verseText.size()){
					endContext = verseText.size() - 1;
				}else{
					endContext = wordIndex + contextFactor + verseText.substr(wordIndex, wordIndex + contextFactor).rfind(" ") + 1;
				}
				ret.push_back(refs[refsIndex] + ":" + verseText.substr(startContext, endContext));
			}
		refsIndex++;
		}
	}
	return ret;
}

std::deque<int> find_substring_positions(std::string str, std::string search, std::deque<char> add){
	std::deque<int> ret;
	for(int i = 0; i < str.size(); i++){
		if(i + search.size() - 1 < str.size() && str.substr(i, search.size()) == search){
			for(char &j : add){
				if(str[i + search.size()] == j){
					ret.push_back(i);
					break;
				}
			}
		}
	}
	return ret;
}

std::deque<std::string> find_references_for_word(std::string word, std::string sourceFile){
	std::deque<std::string> ret; std::deque<std::string> parsedLine;
	std::ifstream readFile; readFile.open(sourceFile); std::string line;
	while(getline(readFile, line)){
		parsedLine = parse_line(sanitize_string(line.substr(line.find("|") + 1, line.size() - 1)));
		for(std::string &i : parsedLine){
			if(i == word){
				ret.push_back(line.substr(0, line.find("|")));
			}
		}
	}
	return ret;
}

std::deque<std::string> parse_line(std::string line){
	std::deque<std::string> ret;
	int wordStart;
	while(1){
		wordStart = line.find(" ");
		if(wordStart > 0){
			ret.push_back(line.substr(0, wordStart));
			line.erase(0, wordStart + 1);
		}else{
			ret.push_back(line);
			return ret;
		}
	}
	return ret;
}

std::deque<struct wordFreq> sort_deque(std::deque<struct wordFreq> dqe){
	bool sorted = false; struct wordFreq hold;
	while(!sorted){
		for(int i = 0; i < dqe.size(); i++){
			for(int j = 0; j < dqe.size(); j++){
				if(dqe[i].word < dqe[j].word){
					sorted = false;
					hold = dqe[j];
					dqe[j] = dqe[i];
					dqe[i] = hold;
				}
			}
		}
		sorted = true;
		for(int i = 0; i < dqe.size(); i++){
			if(i != 0){
				if(dqe[i].word < dqe[i - 1].word){
					sorted = false;
				}
			}
		}
	}
	return dqe;
}

std::string duplicate_string(std::string str, int times){
	std::string ret = "";
	for(int i = 0; i < times; i++){
		ret += str;
	}
	return ret;
}

std::string make_lowercase_string(std::string str){
	std::string ret = "";
	for(char &i : str){
		switch(i){
			case 'A' ... 'Z':
				ret += i + 32;
				break;
			case 'a' ... 'z':
				ret += i;
				break;
			default:
				ret += i;
				break;
		}
	}
	return ret;
}

std::string sanitize_string(std::string str){
	std::string ret = "";
	for(char &i : str){
		switch(i){
			case 'A' ... 'Z':
				ret += i + 32;
				break;
			case ' ':
			case '\'':
			case 'a' ... 'z':
				ret += i;
				break;
		}
	}
	return ret;
}
