#include "pch.h"
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <ctype.h>
#include <vector>
#include <string>
#include <time.h>
#include <tuple>
#include <cmath>

//----------------------------------------CLASSES---------------------------------------*/
class Square { // Gets all essential info for each square
	std::string bonus = " ";
	char letter = ' ';

public:
	void setBonus(std::string str) {
		bonus = str;
	}

	std::string getBonus() {
		return bonus;
	}

	void setLetter(char c) {
		letter = c;
	}

	char getLetter() {
		return letter;
	}

	std::string getContent() {	// Returns the contents of each square, prioritizes letters, then bonus and finally empty spaces
		if (letter != ' ') {
			std::string fLetter;
			fLetter.push_back(int(letter) - 32);	// Makes the letter as upper case, just for looks
			fLetter.push_back(' ');
			return fLetter;
		}

		else if (bonus != " ") {
			return bonus;
		}

		else {
			return "  ";
		}
	}
};

class Board {
	Square tilesMap[15][15]; // Array of squares

	// Creates the board layout, inserting in the correct tiles the corresponding multipliers
	void createBoard() {
		for (int cY = 0; cY < 15; cY++) {
			for (int cX = 0; cX < 15; cX++) {
				int cXY = cX + cY;	// Sum of both X and Y coordinates
				double res = pow(cX, 2) + pow(cY, 2);	// Gets the sum of the power of both coordinates X and Y

				if (cX == cY || cXY == 14) { // Sets the diagonals bonuses of the board
					if (cX == 0 || cX == 14) { // 3 x Word
						tilesMap[cX][cY].setBonus("3W");
					}

					else if (cX == 5 || cX == 9) { // 3 x Letter
						tilesMap[cX][cY].setBonus("3L");
					}

					else if (cX == 6 || cX == 8) { // 2 x Letter
						tilesMap[cX][cY].setBonus("2L");
					}

					else {
						tilesMap[cX][cY].setBonus("2W"); // 2 x Word
					}
				}

				else if (cXY % 7 == 0) {	// 3 x Word outter ring
					if (cY == 0 || cY == 7 || cY == 14) {
						tilesMap[cX][cY].setBonus("3W");
					}
				}

				else if (res == 26 || res == 82 || res == 194 || res == 250) { // 3 x Letter 2nd ring
					if (cY == 1 || cY == 5 || cY == 9 || cY == 13) {
						tilesMap[cX][cY].setBonus("3L");
					}
				}

				else if (res == 9 || res == 121 || res == 205 || res == 317) { // 2 x Letter outer ring
					if (cY == 0 || cY == 3 || cY == 11 || cY == 14) {
						tilesMap[cX][cY].setBonus("2L");
					}
				}

				else if (res == 40 || res == 68 || res == 180 || res == 208) { // 2 x Letter 2nd ring

					if (cY == 2 || cY == 6 || cY == 8 || cY == 12) {
						tilesMap[cX][cY].setBonus("2L");
					}

				}

				else if (res == 58 || res == 170) { // 2 x Letter 3rd ring
					if ((cY == 3) || (cY == 7) | (cY == 11)) {
						tilesMap[cX][cY].setBonus("2L");
					}
				}
			}
		}
	}

public:
	// Calls createBoard() to save the tiles inside the tilesMap[][]
	Board() {
		createBoard();
	}

	// Inserts the letters in a word into the board
	void insertWord(int x, int y, std::string word, std::string orientation) {
		bool insert = false;
		for (char c : word) {
			if (orientation == "h") {	// Horizontal words
				tilesMap[x][y].setLetter(c);
				x++;
			}

			else if (orientation == "v") {	// Vertical words
				tilesMap[x][y].setLetter(c);
				y++;
			}
		}
	}

	// Gets the letters inside a tile, if any of the letters is present on the word it returns true and the letter in that spot, else false
	std::vector<std::tuple<bool, char>> checkTile(int x, int y, std::string word, std::string orientation) {
		std::vector<std::tuple<bool, char>> myVec;
		char cha;
		bool flag = false;
		for (char c : word) {
			if (orientation == "h") {	// Horizontal words
				cha = tilesMap[x][y].getLetter();
				if (cha == c) {	// As soon as the wanted letter is found, it makes the flag as true
					flag = true;
				}
				else {	// If the character is empty or different, flag is false
					flag = false;
					if (cha != ' ') {
						break;
					}
				}
				myVec.push_back(std::make_tuple(flag, cha));
				x++;
			}
			else if (orientation == "v") {	// Vertical words
				cha = tilesMap[x][y].getLetter();
				if (cha == c) {	//  As soon as the wanted letter is found, it makes the flag as true
					flag = true;
				}
				else {	// If the character is empty or different, flag is false
					flag = false;
					if (cha != ' ') {
						break;
					}
				}
				myVec.push_back(std::make_tuple(flag, cha));
				y++;
			}
		}
		return myVec;
	}

	// Returns the value of each letter, being used in wordScore() and Game createDictionary() methods
	int letterScore(char c) {
		int lScore = 0;
		if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'l' || c == 'n' || c == 's' || c == 't' || c == 'r') {
			lScore += 1;
		}
		else if (c == 'd' || c == 'g') {
			lScore += 2;
		}
		else if (c == 'b' || c == 'c' || c == 'm' || c == 'p') {
			lScore += 3;
		}
		else if (c == 'f' || c == 'h' || c == 'v' || c == 'w' || c == 'y') {
			lScore += 4;
		}
		else if (c == 'k') {
			lScore += 5;
		}
		else if (c == 'j' || c == 'x') {
			lScore += 8;
		}
		else if (c == 'q' || c == 'z') {
			lScore += 10;
		}
		else if (c == ' ') {
			lScore += 0;
		}
		return lScore;
	}

	// Calculates the final word score, accounting for letter bonus first and word bonus second
	int wordScore(int x, int y, std::string word, std::string orientation) {
		std::string bonus;
		int wScore = 0;
		int lScore = 0;
		int initX = x;
		int initY = y;
		if (orientation == "h") {	// Calculates horizontal word values
			for (char c : word) {	// Accounts for letter multiplier first
				bonus = tilesMap[x][y].getBonus();
				lScore = letterScore(c);

				if (bonus == "2L") {
					lScore *= 2;
				}
				else if (bonus == "3L") {
					lScore *= 3;
				}
				wScore += lScore;
				x++;
			}
			x = initX;
			for (char c : word) {	// Lastly accounts for word multipliers
				bonus = tilesMap[x][y].getBonus();
				if (bonus == "2W") {
					wScore *= 2;
				}
				else if (bonus == "3W") {
					wScore *= 3;
				}
				x++;
			}
		}
		else if (orientation == "v") {	// Calculates vertical word values
			for (char c : word) {	// Accounts for letter multiplier first
				bonus = tilesMap[x][y].getBonus();
				lScore = letterScore(c);

				if (bonus == "2L") {
					lScore *= 2;
				}
				else if (bonus == "3L") {
					lScore *= 3;
				}
				wScore += lScore;
				y++;
			}
			y = initY;
			for (char c : word) {	// Lastly accounts for word multipliers
				bonus = tilesMap[x][y].getBonus();

				if (bonus == "2W") {
					wScore *= 2;
				}
				else if (bonus == "3W") {
					wScore *= 3;
				}
				y++;
			}
		}
		std::cout << "\nRound score: " << wScore << "\n";
		return wScore;
	}
	
	// Prints the main game board when called
	void drawBoard() {
		int c = 0;
		std::string num;
		std::cout << "\n       1    2    3    4    5    6    7    8    9   10   11   12   13   14   15 ";
		std::cout << "\n    +--------------------------------------------------------------------------+\n";
		for (int cY = 0; cY < 15; cY++) {
			if (cY < 9) {
				std::cout << "  " << cY + 1 << " ";
			}
			else {
				std::cout << " " << cY + 1 << " ";
			}
			for (int cX = 0; cX < 15; cX++) {
				std::cout << "| " << tilesMap[cX][cY].getContent() << " ";	// Gets the content inside the square in the [cX][cY] coordinates
			}
			std::cout << "|";
			if (c == 14) {
				std::cout << "\n    +--------------------------------------------------------------------------+\n";
			}
			else {
				std::cout << "\n    |--------------------------------------------------------------------------|\n";
			}
			c++;
		}
	}
};

class Player {
	int pScore;

public:
	std::vector <std::string> playerHand;

	void setScore(int score) {
		pScore += score;
	}

	int getScore() {
		return pScore;
	}
};

class NPC {
	int npcScore;
	
public:
	std::vector <std::string> npcHand;

	void setScore(int score) {
		npcScore += score;
	}

	int getScore() {
		return npcScore;
	}
};

class Game {
	Square myTile;
	Board myBoard;
	Player myPlayer;
	NPC myNpc;

	bool hasLetter = false;
	bool firstMove = true;
	bool isLegal = false;

	int x = 0, y = 0, l = 0, pCount = 0, moveCounter = 0;

	std::string pName, pInput, orientation;

	std::vector <std::string> boardWords;
	std::vector <std::string> possibleWords;
	std::vector <std::string> pList;
	std::vector <std::string> letterPool = {
	"a", "a", "a", "a", "a", "a", "a", "a", "a", "b",
	"b", "c", "c", "d", "d", "d", "d", "e", "e", "e",
	"e", "e", "e", "e", "e", "e", "e", "e", "e", "f",
	"f", "g", "g", "g", "h", "h", "i", "i", "i", "i",
	"i", "i", "i", "i", "i", "j", "k", "l", "l", "l",
	"l", "m", "m", "n", "n", "n", "n", "n", "n", "o",
	"o", "o", "o", "o", "o", "o", "o", "p", "p", "q",
	"r", "r", "r", "r", "r", "r", "s", "s", "s", "s",
	"t", "t", "t", "t", "t", "t", "u", "u", "u", "u",
	"v", "v", "w", "w", "x", "y", "y", "z", "_", "_"
	};

public:
	std::vector <std::tuple<int, std::string>> lowerDictionary;	// Words < 8
	std::vector <std::tuple<int, std::string>> upperDictionary;	// Words >= 8

	// Prints out any referenced vector of strings
	void printVector(std::vector<std::string> vec) {
		for (int i = 0; i < vec.size(); i++) {
			std::cout << vec[i] << ", ";
		}
		std::cout << "\n";
	}

	// Returns the dictionary with all legal words and respective base scores
	void createDictionary(){
		int  wScore, counter;
		std::string fWord;

		// Open text file and insert words into vector if they are legal
		std::ifstream scrabbleFile("../Scrabble Dictionary/Scrabble Dictionary/scrabble_legal_words.txt");

		while (scrabbleFile >> fWord) {		// Filter, score and add words to dictionary
			counter = 0;
			wScore = 0;
			for (char c : fWord) {	//Gets all the word scores and sends them into the dictionary along with the words
				wScore += myBoard.letterScore(c);
				counter += 1;
				if (counter == fWord.length()) {	// Add word and respective score to the vector wsDictionary
					if (counter <= 7) {
						lowerDictionary.emplace_back(wScore, fWord);
					}
					else {
						upperDictionary.emplace_back(wScore, fWord);
					}
				}
			}
		}
		scrabbleFile.close();
	}

	// Fills player hand with 7 characters from the letterPool
	std::vector <std::string> createHand(std::vector <std::string> pHand) {
		int spaces = 0;
		std::string blank;
		for (int c = pHand.size(); c < 7; c++) {
			if (letterPool.size() > 0) {	// If there are letters in the letter pool
				int num = rand() % letterPool.size(); // Generates a random number between 0 and the letter pool size
				pHand.push_back(letterPool[num]);	// Inserts into the player hand the characters on the letter pool
				letterPool.erase(letterPool.begin() + num);	// Deletes the letter from the num position
			}
			else if(letterPool.size() <= 0 && myPlayer.playerHand.size() <= 0) {	// If there arent any letters on the pool and the player has no letters either the game ends
				finishGame();
				break;
			}			
		}
		//std::cout << "\nLetter pool: " <<letterPool.size() << "\n";
		return pHand;
	}

	// Allows user to input multiple players, non NPC (finish this)
	void addPlayer() {
		/*while (true) {
			std::cout << "How many players? \n";
			std::cin >> pCount;
			if (pCount > 0 && pCount < 5) {
				for (int c = 0; c < pCount; c++) {*/
		std::cout << "\nEnter player name to start: ";
		std::cin >> pName;
		pList.push_back(pName);
		pList.push_back("NPC");

		//if (c == 0) {
		myPlayer.playerHand = createHand(myPlayer.playerHand);
		myNpc.npcHand = createHand(myNpc.npcHand);
		
		/*}
		else if (c == 1) {
			p2Hand = myPlayer.Hand();
		}
		else if (c == 2) {
			p3Hand = myPlayer.Hand();
		}
		else {
			p4Hand = myPlayer.Hand();
		}
		}
		}
		else {
		std::cout << "Error! Number of player inserted not available. (1-4 players) \n";
		}
		}*/
	}

	// When the player uses a wildcard this function is called which allows the player to choose the letter to play in that word
	void wildCard() {
		int cnt = 0;
		std::string word = pInput;
		std::vector <std::string> tempHand;

		if (moveCounter % 2 == 0) {	// Sets the tempHand with the hand depending on the round
			tempHand = myPlayer.playerHand;
		}
		else {
			tempHand = myNpc.npcHand;
		}

		for (auto c : pInput) {	// When the player uses a wild card it asks for what letter it wants to change
			std::string ch;
			ch = "";
			if (c == '_') {
				std::cout << "\nWhich character do you want to replace the wild card with?\n";
				std::cin >> c;
				if (int(c) >= 97 && int(c) <= 122) {	// If the chosen character is valid [a - z]			
					ch.push_back(c);
					word.at(cnt) = c;	// Replaces the wildcard at the cnt spot, with the new character

					for (int i = 0; i < tempHand.size(); i++) {	// Replaces all the wildcards in the hand with the inserted letters
						if (tempHand[i] == "_") {
							if (moveCounter % 2 == 0) {
								tempHand.at(i) = ch;	// Replaces the wild card in the player hand
								break;
							}
							else {
								tempHand.at(i) = ch;	// Replaces the wild card in the player hand
								break;
							}
						}
					}									
				}
				else {
					std::cout << "\nInput a valid character [a - z]\n";
					wildCard();
				}				
			}
			cnt++;
		}
		if (checkWords(word) == true) {	// If the final word is in the dictionary makes the changes bellow
			pInput = word;
			if (moveCounter % 2 == 0) {
				myPlayer.playerHand = tempHand;
			}
			else {
				myNpc.npcHand = tempHand;
			}
		}
	}

	// Gets player inputs, checks and inserts them into the board
	void askPlayerInput() {
		while (isLegal == false) {
			pInput = "";
			std::cout << "\nInput your word: ";
			std::cin >> pInput;			

			if (pInput == "1") {		// Debug word
				std::string ch;
				pInput = "hello";
				for (char c : pInput) {
					ch = "";
					ch.push_back(c);
					myPlayer.playerHand.push_back(ch);
				}
			}

			else if (pInput == "pass") {
				break;
			}

			wildCard();	// Asks for player input when he uses a wildcard "_"			
			isLegal = checkWords(pInput); // Checks if the words are legal comparing the word with the dictionary			

			if (moveCounter != 0 && pInput.size() == 1) {
				isLegal = false;
			}

			if (isLegal == true) {	// If word is legal, it will check the coordinates on the board and compare the input with the player hand
				if (moveCounter == 0) {	// 1st round
					hasLetter = compareHand(pInput, myPlayer.playerHand);
					checkPlayerCoords();
				}
				else {	// All other rounds
					checkPlayerCoords();
					hasLetter = compareHand(pInput, myPlayer.playerHand);
				}

				if (hasLetter == true) {	//	If all the letters are in the hand, it inserts the word in the board
					std::cout << "\nWord inserted successfully\n";
					boardWords.push_back(pInput);
					myBoard.insertWord(x, y, pInput, orientation);
					if (moveCounter % 2 == 0) {
						myPlayer.setScore(myBoard.wordScore(x, y, pInput, orientation)); // Increments the player score

					}
					else {
						myNpc.setScore(myBoard.wordScore(x, y, pInput, orientation)); // Increments the npc score)
					}
					break;
				}
				else { // Prints an error and asks for input again, isLegal gets reset
					std::cout << "\nWord letters do not match player hand, please try again\n";
					isLegal = false;
				}
			}
			else {	// If the word isn't legal, prints a message and the hand
				std::cout << "\nWord inserted is not legal, please try again\n";
			}
		}
		isLegal = false;
		while (myPlayer.playerHand.size() > 7) {	// If the player hand is bigger than 7 removes the extra characters
			myPlayer.playerHand.pop_back();
		}
	}

	// Sets first play of the game and gets player inserted coordinates and checks if they are valid
	void checkPlayerCoords() {
		std::vector<std::tuple<bool, char>> myTuple;
		std::string fChar;
		bool next = false;
		bool newInput = false;
		int interCounter = 0, checkCounter = 0;
		x = y = 0;

		if (moveCounter == 0) {	// This becomes the first play in the game, if it's true then coords are going to be [7][7]
			std::string c;
			while (true) {	// While orientation isn't one of the two bellow, keeps asking for input
				std::cout << "\nInput the word orientation (horizontal / vertical): ";
				std::cin >> orientation;
				orientation = tolower(orientation[0]);	// Gets first char of the orientation in lower case
				if (orientation == "v" || orientation == "h") {
					x = y = 7;
					break;
				}
				else {
					std::cout << "\nOrientation inserted not available (try h or v)";
				}
			}
		}
		else {	// If the word inserted isn't the first one the player is able to choose the coordinates in which to play
			while (true) {
				std::cout << "\nInput X coordinate (1 - 15): ";
				std::cin >> x;
				std::cout << "\nInput Y coordinate (1 - 15): ";
				std::cin >> y;
				std::cout << "\nInput the word orientation (horizontal / vertical): ";
				std::cin >> orientation;
				orientation = tolower(orientation[0]);	// Gets first char of the orientation in lower case

				if (y < 15 && y > 0 || x < 15 && x > 0) {
					x -= 1; y -= 1;
					if (orientation == "h") {
						if (x + pInput.length() > 0 && x + pInput.length() <= 15) {	// To make sure the word the player inserted does not exceed the board limits
							myTuple = myBoard.checkTile(x, y, pInput, orientation);	// Returns a tuple with a bool and a char

							if (myTuple.size() == pInput.size()) {	// If the tuple is the same size as the word, the checkTile successfully checked all the letters
								for (auto i : myTuple) {
									fChar = "";
									if (std::get<0>(i) == true) {
										fChar.push_back(std::get<1>(i));
										myPlayer.playerHand.emplace_back(fChar);	// The char will be added to the player hand so that the word may pass the compareHand()
										checkCounter++;
									}
									else {
										interCounter++;	// Counts the number of times the False from the tuple was accounted for
									}
								}
								if (interCounter == pInput.length() || checkCounter == pInput.length()) {	// If the counter is the size of the word it will not allow the word to be inserted
									std::cout << "\nUnable to add word to the indicated coordinates, no words nearby/Intersected letters are different\n";
								}
								else {	// IF it isn't the same size it will add pass the check
									break;
								}
							}
							else {	//If the tuple size is lower than the word it means that the checkTile had a match with a different letter
								std::cout << "\nCould not insert, word is trying to overlay different letters\n";
							}
						}
						else {	// if the inserted coordinates exceed the limits, it asks to introduce coords again
							std::cout << "\nCoordinates entered exceed the board limits! Try coordinates [1-15]\n";
						}
					}

					else if (orientation == "v") {
						if (y + pInput.length() > 0 && y + pInput.length() <= 15) {	// To make sure the word the player inserted does not exceed the board limits
							myTuple = myBoard.checkTile(x, y, pInput, orientation);	// Returns a tuple with a bool and a char

							if (myTuple.size() == pInput.size()) {	// If the tuple is the same size as the word, the checkTile successfully checked all the letters
								for (auto i : myTuple) {
									fChar = "";
									if (std::get<0>(i) == true) {	// If the letter of the word is in the board, the tuple will return true
										fChar.push_back(std::get<1>(i));
										myPlayer.playerHand.emplace_back(fChar);	// The char will be added to the player hand so that the word may pass the compareHand()
										checkCounter++;
									}
									else {
										interCounter++;
									}
								}
								if (interCounter == pInput.length() || checkCounter == pInput.length()) {	// If the counter is the size of the word it will not allow the word to be inserted
									std::cout << "\nUnable to add word to the indicated coordinates, no words nearby/Intersected letters are different\n";
								}
								else {	// IF it isn't the same size it will add pass the check
									break;
								}
							}
							else {	//If the tuple size is lower than the word it means that the checkTile had a match with a different letter
								std::cout << "\nCould not insert, word is trying to overlay different letters\n";
							}
						}
						else {	// if the inserted coordinates exceed the limits, it asks to introduce coords again
							std::cout << "\nCoordinates entered exceed the board limits! Try coordinates [1-15]\n";
						}
					}
					else {
						std::cout << "\nOrientation inserted not available (try h or v): ";
					}
				}
				else {
					std::cout << "\nUnavailable coordinates inserted! Try coordinates [1-15]\n";
				}
			}
		}
	}

	// Checks if a word is legal or not and return true or false according
	bool checkWords(std::string Input) {
		bool valid = false;

		if (pInput.size() <= 7) {
			for (const auto& i : lowerDictionary) {	// Checks all the words in the dictionary to see if the input is present
				if (Input == std::get<std::string>(i)) {	// If the word is present returns true
					valid = true;
					break;
				}
				else {	// If not present, returns false and a message
					valid = false;
				}
			}
		}
		else{
			for (const auto& i : upperDictionary) {	// Checks all the words in the dictionary to see if the input is present
				if (Input == std::get<std::string>(i)) {	// If the word is present returns true
					valid = true;
					break;
				}
				else {	// If not present, returns false and a message
					valid = false;
				}
			}
		}		
		return valid;
	}

	// Get player word and hand as inputs and return if the letters in the word match any of the player hand
	bool compareHand(std::string word, std::vector<std::string> pHand) {
		std::vector<std::string> tempHand = pHand;
		int counter = 0, removeElem = 0;
		bool hasLetters = false;
		std::string l;

		for (int i = 0; i < word.length(); i++) {	// For all letters in the word
			l = "";
			l.push_back(word[i]);

			for (int j = 0; j < pHand.size(); j++) {	// For all letters in hand
				if (l == pHand[j]) {
					pHand.push_back(l);	// Adds the matching letters to the end of the hand
					pHand.erase(pHand.begin() + j);	// Remove letter from hand at j index if it matches word letter
					counter ++;
					removeElem++;
					break;
				}
			}
		}

		for (int i = removeElem; i > 0; i--) {	// Remove the letters that were added to the end of the hand
			pHand.pop_back();
		}

		if (counter >= word.length()) {	// Returns true if the counter is bigger than or the same size of the word and creates a new player hand
			pHand = createHand(pHand);
			if (moveCounter % 2 == 0) {
				myPlayer.playerHand = pHand;
			}
			else {
				myNpc.npcHand = pHand;
			}
			hasLetters = true;
		}
		else {	// If all the letters in the word aren't in the hand, returns false and makes the player hand equal to it's initial state
			if (moveCounter % 2 == 0) {
				myPlayer.playerHand = tempHand;
			}
			else {
				myNpc.npcHand = tempHand;
			}
			hasLetters = false;
		}
		return hasLetters;
	}

	// -----------------------------------------BOT CODE------------------------------------------------
	// Checks the player input and gets the bot word
	void checkBotWords() {
		std::string botWord;
		std::vector<std::tuple<bool, char>> myVec;

		int wScore = 0, tCounter = 0, botX = x, botY = y;
		
		if (orientation == "h") {	// If player plays horizontally the bot plays vertically
			orientation = "v";
			while (true) {
				botY = y;
				if (pInput.size() <= 7) {
					botWord = possibleNpcWord(lowerDictionary);
				}
				else {
					botWord = possibleNpcWord(upperDictionary);
				}
				for (char c : botWord) {	// To get the starting position at which the bot can insert the word for Vertical
					if (c != pInput[l]) {	// If it doesnt match the starting character of the player word, decrements the y
						botY--;
					}
					else {
						break;
					}
				}

				if (botY < 16 && botY > 0) {	// If the coordinate is valid passes the check
					break;
				}			
			}
			std::cout << "\nBot word: " << botWord << "\n";
			if (botWord != "pass") {	// If the word is valid, removes the letters from the hand, inserts the word in the board and increments the bot score
				compareHand(botWord, myNpc.npcHand);
				myBoard.insertWord(botX, botY, botWord, orientation);
				wScore = myBoard.wordScore(botX, botY, botWord, orientation);
				myNpc.setScore(wScore);
			}			
		}
		else {	// If player plays vertically the bot plays horizontally
			orientation = "h";
			while (true) {
				botY = y;
				if (pInput.size() <= 7) {
					botWord = possibleNpcWord(lowerDictionary);
				}
				else {
					botWord = possibleNpcWord(upperDictionary);
				}
				for (char c : botWord) {	// To get the starting position at which the bot can insert the word for Vertical
					if (c != pInput[l]) {	// If it doesnt match the starting character of the player word, decrements the y
						botY--;
					}
					else {
						break;
					}
				}
				if (botY < 16 && botY > 0) {	// If the coordinate is valid passes the check
					break;
				}
			}
			std::cout << "\nBot word: " << botWord << "\n";
			if (botWord != "pass") {	// If the word is valid, removes the letters from the hand, inserts the word in the board and increments the bot score
				compareHand(botWord, myNpc.npcHand);
				myBoard.insertWord(botX, botY, botWord, orientation);
				wScore = myBoard.wordScore(botX, botY, botWord, orientation);
				myNpc.setScore(wScore);
			}
		}
		while (myNpc.npcHand.size() > 7) {	// If the bot hand is bigger than 7 removes the extra characters
			myNpc.npcHand.pop_back();
		}
		l = rand() % pInput.size();
	}

	// Searches all the words in the dictionaries, makes a check on them, inserts them into a possible words vector and then chooses a word
	std::string possibleNpcWord(std::vector<std::tuple<int, std::string>> myDict) {
		possibleWords.clear();
		std::vector <std::string> tempHand;
		std::string ch, nWord, botWord;
		int checkInt = 0, myRand = 0;

		ch.push_back(pInput[l]);
		myNpc.npcHand.push_back(ch);	// Pushing the l character of the word into the hand

		tempHand = myNpc.npcHand;

		for (const auto& word : myDict) {	// Gets the dictionary word
			nWord = std::get<std::string>(word);
			checkInt = 0;
			tempHand = myNpc.npcHand;

			for (char c : nWord) {	// Each character in the word
				ch.clear();
				ch.push_back(c);
				
				for (int i = 0; i < tempHand.size(); i++) {	// Compares it to the npc hand
					if (tempHand[i] == ch) {	// If the characters match
						tempHand.erase(tempHand.begin() + i);
						checkInt++;
						break;
					}
					else if (tempHand[i] == "_") {	// If the character in the hand is a wildcard
						tempHand.erase(tempHand.begin() + i);
						checkInt++;
						break;
					}
				}					
			}
			if (checkInt == nWord.size() && checkInt > 1) {	// If there are all the letters in the hand
				for (char c : nWord) {
					if (c == pInput[l]) {
						possibleWords.push_back(nWord);
					}
				}
			}
		}

		if (possibleWords.size() > 0) {	// If there are word in the possibleWords vector
			myRand = rand() % possibleWords.size();
			botWord = possibleWords[myRand];
		}
		else {	// If the vector is empty the word becomes a pass
			botWord = "pass";
		}

		return botWord;
	}
	//-------------------------------------------------------------------------------------------------

	// Controls player and NPC turns
	void gameTurn() {
		bool end = false;
		int tempMoveCounter = moveCounter + 1;
		while (end == false) {
			if (moveCounter % 2 == 0) { // Player's turn
				
				//------------------------Visual--------------------------------
				std::cout << "\n*------------------------ Round: " << tempMoveCounter << " | Player: " << pList[moveCounter % 2] << " | Score: " << myPlayer.getScore() << " ----------------------------*\n";
				myBoard.drawBoard();
				std::cout << "\nPlayer Hand: ";
				printVector(myPlayer.playerHand);
				askPlayerInput();
				//--------------------------------------------------------------
			}
			else {	// NPC's turn
				//------------------------Visual--------------------------------
				std::cout << "\n*------------------------ Round: " << tempMoveCounter << " | Player: " << pList[moveCounter % 2] << " | Score: " << myNpc.getScore() << " ----------------------------*\n";
				myBoard.drawBoard();
				std::cout << "\nNPC Hand: ";
				printVector(myNpc.npcHand);
				checkBotWords();
				//--------------------------------------------------------------
			}
			moveCounter += 1;
		}
	}

	// Calls all the starting functions
	void start() {
		addPlayer();
		gameTurn();
	}

	// Prints a few lines depending on the finishing condition
	void finishGame() {		
		std::cout << "\nGame finished!!\n";

		if (myPlayer.getScore() > myNpc.getScore()) {
			std::cout << "The winner is: " << pList[0] << "\n";
		}
		else if (myPlayer.getScore() < myNpc.getScore()) {
			std::cout << "The winner is: " << pList[1] << "\n";
		}
		else {
			std::cout << "It's a tie!!";
		}
		std::cout << "\nPress any key to exit...\n";
		std::cin.ignore();
		std::cin.get();
		exit(0);
	}
};

int main() {
	Game mainGame;
	mainGame.createDictionary();

	//------------------------VARIABLES-----------------------------*/
	srand(time(0)); // Gets a random value each processor clock tick, needed for creating player hand

	std::vector <std::tuple<int, std::string>> wsDictionary;
	//---------------------INTRODUCTION/RULES-----------------------*/
	std::cout << "*----------------------------------------------------------------------------------------*";
	std::cout << "\n\t\t\t\tWELCOME to Scrabble the PC Game!\n";
	std::cout << "*----------------------------------------------------------------------------------------*";
	std::cout << "\nObjective:\n";
	std::cout << " You will play against a NPC that is going to try and beat you on a 1 VS 1 scrabble match.\n You are supposed to score the most points against the bot.\n Watch out for word bonuses on the board, try and hit as many as you can with 1 play.\n The game ends when neither player or the bot can play, or there are no letters left.\n";
	std::cout << "*----------------------------------------------------------------------------------------*";
	std::cout << "\nRules:\n";
	std::cout << " 1. With your letters try and make a word to insert on the board;\n 2. You can only play words attached to existing ones through duplciate letters;\n 3. Score as many points as you can;\n 4. Have fun!!\n";
	std::cout << "*----------------------------------------------------------------------------------------*";
	std::cout << "\n There are: " << mainGame.lowerDictionary.size() + mainGame.upperDictionary.size() << " legal words in the dictionary.\n";
	std::cout << "*----------------------------------------------------------------------------------------*\n";
	mainGame.start();
	//--------------------------------------------------------------*/
}