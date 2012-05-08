/*
 *  File: Random Writer.cpp
 *  ------------------
 *
 *
 *	This program reads in a text provided by the user and does a user-specified k degree Markov model
 *	analysis of the text. It then provides a randomly-generate text passage.
 */

#include "genlib.h"
#include "simpio.h"
#include "vector.h"
#include "map.h"
#include "random.h"
#include <string>
#include <iostream>
#include <fstream>

const int maxPrintChars = 2000;

/**
 * Function: GetFileName
 * -----------------------
 * Reads in a text file specified by the user.
 *
 * @param & sourceText uses the text specified by the user to open the ifstream
 *						that is passed in.
 */
void GetFileName(ifstream & sourceText) 
{
	cout << "Please enter filename containing source text: ";
	string text = GetLine();
	sourceText.open(text.c_str());
	while (sourceText.fail()) {
		sourceText.clear();
		cout << "Unable to open file named " << text << ". Please try again." << endl;
		cout << "Please enter the filename containing source text: ";
		text = GetLine();
		sourceText.open(text.c_str());
	}
}

/**
 * Function: GetMarkovModel
 * --------------------------
 * Determines the k degree of Markov model analysis based on user input.
 *
 * @return model which determines the k degree analysis to be done.
 */
int GetMarkovModel()
{
	cout << "What order of analysis? (a number from 1 - 10): ";
	int model = GetInteger();
	while (model < 1 | model > 10) {
		cout << "Please enter a number from 1 - 10: ";
		model = GetInteger();
	}
	
	return model;
}	

/**
 * Function: AnalyzeSourceText
 * -----------------------------
 * Takes in the ifstream of the text and does a Markov model analysis.
 *
 * @param sourceText		reads from the ifstream and closes it once it is done.
 * @param model				user-specified k degree Markov model
 * @return charOccurances	the generated Map<Vector<char> > containing the
 *							char sequences that appear in the sourceText stored in
 *							Vectors.
 */
Map<Vector<char> > AnalyzeSourceText(ifstream & sourceText, int model)
{	
	Map<Vector<char> > charOccurances;
	while (true) {
		int nextChar;
		string charSequence = "";
		for (int i = 0; i < model; i++) {
			nextChar = sourceText.get();
			if (nextChar == EOF) { //reached end of file
				sourceText.close();
				return charOccurances;
			}
			charSequence += char(nextChar);
		}
		char followingChar = sourceText.get();
		if (followingChar != EOF) {//make sure the char was not the end of the file
			if (charOccurances.containsKey(charSequence))	{ //checks if char sequence has previously occured
				charOccurances[charSequence].add(followingChar);
			} else {
				Vector<char> newCharSequence;
				newCharSequence.add(followingChar);
				charOccurances[charSequence] = newCharSequence;
			}
		} else {
			return charOccurances;
		}
		for (int j = 0; j < model; j++) {
			sourceText.unget();
		}
	}
}

/**
 * Function: MostFrequentSequence
 * -------------------------------
 * Iterates through the Map to find the key with the greatest number of
 * elements in its Vector value.
 *
 * @param charOccurances the Map that the iterator iterates through.
 * @return mostFrequent a string containing the key with the greatest
 *						number of elements in its corresponding Vector.
 *						the more elements in the Vector, the more often the
 *						key occurred in the text.
 */
string MostFrequentSequence(Map<Vector<char> > charOccurances)
{	
	Map<Vector<char> >::Iterator it = charOccurances.iterator();
	string mostFrequent = it.next();
	while (it.hasNext()) {
		string key = it.next();
		if (charOccurances[key].size() > charOccurances[mostFrequent].size()) {//compares size of the 2 vectors
			mostFrequent = key;
		}
	}
	
	return mostFrequent;
}

/**
 * Function: main
 * ---------------
 * Serves as the entry point of the program. First, reads user input. Second, analyzes
 * the text and stores the data into a Map<Vector<char> >. Third, prints out a sequence
 * of chars using a randomly-generated integer, which is then used as the index value of
 * the Vector corresponding to the previous charSequence key.
 *
 * @return 0 if the program is successfully completed
 */
int main()
{	
	Randomize(); //initializes a random sequence
	ifstream sourceText;
	GetFileName(sourceText);
	int model = GetMarkovModel(); 
	Map<Vector<char> > charOccurances = AnalyzeSourceText(sourceText, model);
	string charSequence = MostFrequentSequence(charOccurances);
	int numPrintedChars = model;
	char randomChar;
	cout << charSequence; //first prints the most frequentlly occuring sequence
	while (numPrintedChars < maxPrintChars) {
		if (charOccurances.containsKey(charSequence)) {
			Vector<char> charVector = charOccurances[charSequence];
			randomChar = charVector[RandomInteger(0, charVector.size() - 1)];
		} else break;	//if the map does not contain the key, the char sequence only occured at the end of the text
        // this means there is no char following this sequence and the program ends early.
		cout << randomChar;
		numPrintedChars++;
		charSequence = charSequence.substr(1, charSequence.size() - 1) + randomChar;
        //the new key becomes the next k-number sequence of chars in the text
	}
	return(0);
}


