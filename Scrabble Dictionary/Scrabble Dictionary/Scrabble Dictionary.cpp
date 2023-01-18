#include "pch.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <vector>
#include <string>
#include <tuple>

// Returns the dictionary with all legal words and respective base scores
void createDictionary() {
	std::vector <std::string> wscoreDictionary;

	int  wScore, counter;
	std::string fWord;

	// Open text file and insert words into vector if they are legal
	std::ifstream scrabbleFile("scrabble_words.txt");
	std::ofstream s_l_words("scrabble_legal_words.txt");

	while (scrabbleFile >> fWord) {		// Filter, score and add words to dictionary
		counter = 0;
		wScore = 0;
		for (char c : fWord) {
			if (int(c) >= 97 && int(c) <= 122 && fWord.size() < 16) {	// Check if any digit besides lower case characters using ASCII table as reference (decimal equivalent)
				// Score the words after being filtered
				counter += 1;
			}
			if (counter == fWord.length()) {	// Add word and respective score to the vector wsDictionary
				s_l_words << fWord << "\n";
				wscoreDictionary.emplace_back(fWord);
			}
		}
	}

	std::sort(wscoreDictionary.begin(), wscoreDictionary.end());	// Sorting the dictionary alphabetically
	scrabbleFile.close();
	s_l_words.close();
}

int main()
{
	std::cout << "Inserting legal words in the file...";
	createDictionary();
}