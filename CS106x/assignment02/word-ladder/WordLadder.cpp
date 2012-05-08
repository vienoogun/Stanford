/*
 *  File: WordLadder.cpp
 *  ------------------
 *
 
 *	This program takes in 2 different string inputs from the user and attempts to connect
 *	them using a word ladder, with each subsequent word in the ladder differing from the
 *	previous one by one letter.
 */

#include "genlib.h"
#include "simpio.h"
#include "queue.h"
#include "lexicon.h"
#include "stack.h"
#include "strutils.h"
#include <string>
#include <iostream>

/**
 * Function: GenerateWordLadder
 * ______________________________
 * Manages the queues and stacks that is used to store partial ladders. Is also responsible
 * for incrementing the startWord and finding all possible combinations of words.
 *
 * @param startWord			uses the startWord as the starting point of the ladders.
 * @param destinationWord	uses the destinationWord as a sentinel - once the function is able
 *							to compute the destinationWord, the while loop ends.
 * @param wordLadders		reads from the wordLadder, determines the possible extension of the stacks,
 *							then updates the wordLadder accordingly in a cycle that does not end until
 *							the sentinel is met.
 * @return tempStack		once the sentinel is met, returns the stack that contains the sentinel, which
 *							will also be the shortest word ladder.
 */
Stack<string> GenerateWordLadder(string startWord, string destinationWord, Queue<Stack<string> > wordLadders)
{
	Lexicon wordList("lexicon.dat");
	Lexicon usedWords;
	usedWords.add(startWord);
	while (true) {
		Stack<string> initStack = wordLadders.dequeue();
		Stack<string> tempStack = initStack;
		string word = initStack.peek();
		for (int i = 0; i < word.length(); i++) {
			word = initStack.peek();
			for (int j = 97; j <= 122; j++) { //ASCII code of characters makes sure all letters of the alphabet are used
				string replacement = "";
				replacement += char(j);
				word.replace(i, 1, replacement);
				if (wordList.containsWord(word) && !usedWords.containsWord(word) && word != tempStack.peek()) {
					tempStack = initStack;
					usedWords.add(word);
					tempStack.push(word);
					wordLadders.enqueue(tempStack);
					if (word == destinationWord) return tempStack; //once sentinel is met, return the stack containing it
				}
			}
		}
		if (wordLadders.isEmpty()) { //if wordLadders.isEmpty() all possiblities have been checked without arriving at sentinel
			Stack<string> emptyStack;
			return emptyStack;
		}
	}
}	

/**
 * Function: GetWordPair
 * ----------------------
 * Reads in user input to generate the word pair necessary to form the word ladder.
 *
 * @param & startWord turns the null string into the user-specified beginning word
 * @param & destinationWord turns the null string into the user-specified destination word
 * @return bool true or false depending on user input. returns true if user enters in an input
 *				during the first prompt, else returns false.
 */
bool GetWordPair(string & startWord, string & destinationWord)
{
	cout << "Enter start word (RETURN to quit): ";
	startWord = GetLine();
	if (startWord == "") return false; //signals that user wishes to quit program
	cout << "Enter destination word: ";
	destinationWord = GetLine();
	return true;
}


/**
 * Function: main
 * ----------------
 * Serves as the entry point of the program. First, it gets the user input. Second, it creates an initial
 * Queue<Stack<string> > as well as an initial Stack<string>. Third, it passes those 3 things in
 * as parameters and receives a final Stack<string> in return. Fourth, it prints out a response
 * according to the Stack<string> returned.
 *
 * @return 0 if the program was completed successfully.
 */
int main()
{
	while (true) {
		string startWord = "";
		string destinationWord = "";
		if (!GetWordPair(startWord, destinationWord)) break; //signals that the user wants to end the program
		cout << "Searching..." << endl;
		Queue<Stack<string> > wordLadders;
		Stack<string> initLadder;
		initLadder.push(startWord);
		wordLadders.enqueue(initLadder);
		Stack<string> finalLadder = GenerateWordLadder(startWord, destinationWord, wordLadders);//shortest ladder
		if (finalLadder.isEmpty()) {//means it is impossible to form a ladder between the two words
			cout << "No ladder found." << endl;
		} else {
			cout << "Found ladder:" << endl;
			while (!finalLadder.isEmpty()) cout << finalLadder.pop() << " "; 
		}
        cout << endl;
        cout << endl;
	}
	return (0);
}

