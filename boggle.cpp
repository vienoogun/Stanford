
/* File: boggle.cpp
 * ----------------
 *
 * This program plays the game of boggle according to a set of specified rules. It is a 1 player game,
 * in which the player will face off against the computer (only to lose miserably of course).
 */

#include "genlib.h"
#include "simpio.h"
#include "strutils.h"
#include "random.h"
#include "gboggle.h"
#include "lexicon.h"
#include "grid.h"
#include <iostream>
#include "extgraph.h"
#include "vector.h"
#include "sound.h"

const string BadUserGuessResponse[8] =
{"Guessing already?", "Oh, come on.", "Tryin' to be slick, eh?", "This is embarassing.",
    "Maybe it's time to give up.", "Randomly guessing now, are we?", "Desperation is a sad thing indeed.",
    "Ready to give up yet?"};

const string StandardCubes[16]  = 
{"AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS", "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW", "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"};

const string BigBoggleCubes[25]  = 
{"AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM", "AEEGMU", "AEGMNN", "AFIRSY", 
    "BJKQXZ", "CCNSTW", "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT", "DHHLOR", 
    "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU", "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"};

const int minWordLength = 4;
const double pauseTime = 0.25;

/*
 * Function: Welcome
 * --------------------
 * Introduces the game of boggle to the user.
 */
void Welcome()
{
    cout << "Welcome!  You're about to play an intense game of mind-numbing Boggle. " 
    << "The good news is that you might improve your vocabulary a bit.  The "
    << "bad news is that you're probably going to lose miserably to this little "
    << "dictionary-toting hunk of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: VerifyUserInput
 * ---------------------------
 * Checks to see whether or not user has supplied appropriate input to a "yes" or "no" question.
 */
string VerifyUserInput(string answer)
{
	while (answer != "YES" && answer != "NO") { //prompts user until correct input is required
		cout << "Please answer yes or no: ";
		answer = ConvertToUpperCase(GetLine());
	}
	return answer;
}

/*
 * Function: GiveInstructions
 * ----------------------------
 * Provides user with option to obtain instructions on how to play boggle.
 */
void GiveInstructions()
{	
	cout << "Do you need instructions? ";
	string answer = VerifyUserInput(ConvertToUpperCase(GetLine()));
	if (answer == "YES") {
		cout << endl << "The boggle board is a grid onto which I will randomly distribute " 
		<< "cubes. These 6-sided cubes have letters rather than numbers on the faces, " 
		<< "creating a grid of letters on which you try to form words. You go first, " 
		<< "entering all the words you can find that are formed by tracing adjoining " 
		<< "letters. Two letters adjoin if they are next to each other horizontally, " 
		<< "vertically, or diagonally. A letter can only be used once in the word. Words "
		<< "must be at least 4 letters long and can only be counted once. You score points "
		<< "based on word length: a 4-letter word is worth 1 point, 5-letters earn 2 "
		<< "points, and so on. After your puny brain is exhausted, I, the super computer, "
		<< "will find all the remaining words and double or triple your paltry score." << endl;
		cout << "\nHit return when you're ready...";
		GetLine();
	}
}

/*
 * Function: SoundFeature
 * -------------------------
 * Provides user with option to turn the sound on or off.
 */
void SoundFeature()
{
	cout << endl << "Would you like to turn on the sound? ";
	string answer = VerifyUserInput(ConvertToUpperCase(GetLine()));
	if (answer == "NO") {
		SetSoundOn(false);
	} else {
		SetSoundOn(true);
	}
}

/*
 * Function: BoggleBoardSize
 * ----------------------------
 * Provides user with option to increase dimensions of the boggle board from a 4x4 to a 5x5.
 *
 *@return int which denotes the user-specified grid dimension.
 */
int BoggleBoardSize()
{
	cout << endl << "I'll give you a chance to set up the board to your specification." << endl;
	cout << "Do you want to play on a 5x5 boggle board instead of a 4x4? ";
	string answer = VerifyUserInput(ConvertToUpperCase(GetLine()));
	if (answer == "YES") return 5;
	return 4;
}

/*
 * Function: UserBoardConfiguration
 * -----------------------------------
 * Provides user with option to specify a specific board configuration.
 */
bool UserBoardConfiguration()
{
	cout << "Do you want to force the board configuration? ";
	string answer = VerifyUserInput(ConvertToUpperCase(GetLine()));
	if (answer == "YES") return true;
	return false;
}

/*
 * Function: AdjacentPoint
 * --------------------------
 * Finds the dice adjacent to the specified one.
 *
 *@return bool	which returns false whenever the specified dice is the very last one in the grid.
 *				thus, no adjacent dice exists.
 */
bool AdjacentPoint (int boardRow, int boardCol, int &row, int &col)
{
	if (col < boardRow) col++; 
	if (col >= boardCol) {
		col = 0; 
		row++;
	} 
	if (row >= boardRow) return false;
	return true;
}

/*
 * Function: DrawBoggleBoard
 * ---------------------------
 * Draws the graphic display of the boggle board, reflecting the char configuration of the boggleBoard.
 */
void DrawBoggleBoard(Grid<char> boggleBoard)
{
	int row = 0, col = 0;
	while (true) {
		LabelCube(row, col, boggleBoard.getAt(row, col));	
		if (!AdjacentPoint(boggleBoard.numRows(), boggleBoard.numCols(), row, col)) break;
	}
}

/*
 * Function: GetDiceConfiguration
 * ---------------------------------
 * Prompts user for a specific dice configuration for boggleBoard.
 *
 *@return diceConfig which denotes a string containing the user-specified dice configuration.
 */
string GetDiceConfiguration(int boardDimension)
{
	int maxChars = boardDimension * boardDimension;
	cout << endl << "Enter a " << maxChars << "-character string to identify which letters you want on the cubes." << endl;
	cout << "The first " << boardDimension << " letters are the cubes on the top row from left to right next "
    << boardDimension << " letters are the second row, etc." << endl;
	cout << "Enter the string: ";
	string diceConfig = ConvertToUpperCase(GetLine());
	while (diceConfig.length() < maxChars) { //prompts until dice configuration meets minimum length of maxChars
		cout << "String must include " << maxChars << " characters! Try again: ";
		diceConfig = ConvertToUpperCase(GetLine());
	}
	return diceConfig;
}

/*
 * Function: SetDiceConfiguration
 * ----------------------------------
 * Sets up the boggleBoard configuration of the dice according to the user-specified string of chars.
 */
void SetDiceConfiguration(Grid<char> &boggleBoard, string diceConfig)
{
	int stringIndex = 0, row = 0, col = 0;
	while (true) {
		boggleBoard.setAt(row, col, diceConfig.at(stringIndex));
		stringIndex++;
		if (!AdjacentPoint(boggleBoard.numRows(), boggleBoard.numCols(), row, col)) break;
	}
}

/*
 * Function: ShuffleDice
 * --------------------------
 * Randomly shuffles the dice in the boggleBoard by swapping the contents of 2 random locations in boggleBoard. 
 * Ensures the same dice does not always appear in the same location.
 */
void ShuffleDice(Grid<char> &boggleBoard)
{
	int row = 0, col = 0;
	while (true) {
		int randomRow = RandomInteger(0, boggleBoard.numRows() - 1);
		int randomCol = RandomInteger(0, boggleBoard.numCols() - 1);
		char dice = boggleBoard.getAt(row, col);
		boggleBoard.setAt(row, col, boggleBoard.getAt(randomRow, randomCol));
		boggleBoard.setAt(randomRow, randomCol, dice);
		if (!AdjacentPoint(boggleBoard.numRows(), boggleBoard.numCols(), row, col)) break;
	}
}

/*
 * Function: InitializeRandomBoard
 * ----------------------------------
 * Randomly initializes boggleBoard and picks a random side of the dice to face up by randomly selecting
 * a char from the const dice configuration. Then, calls on ShuffleDice to randomize the positions of the dice.
 */
void InitializeRandomBoard (Grid<char> &boggleBoard)
{
	PlayNamedSound("dice rattle.wav");
	int diceCount = 0, row = 0, col = 0;
	char randomChar;
	while (true) {
		if (boggleBoard.numRows() == 4) { //distinguishes between the 5x5 and 4x4 dice configs
			randomChar = StandardCubes[diceCount].at(RandomInteger(0, 5));
		} else {
			randomChar = BigBoggleCubes[diceCount].at(RandomInteger(0, 5));
		}
		boggleBoard.setAt(row, col, randomChar);
		diceCount++;
		if (!AdjacentPoint(boggleBoard.numRows(), boggleBoard.numCols(), row, col)) break;
	}
	ShuffleDice(boggleBoard);
}

/*
 * Function: CreateMarker
 * ---------------------------
 * Initializes all locations in usedDice to false. Is used in other functions as a marker of
 * whether or not the specific dice has been used in the char arrangement.
 */
void CreateMarker(Grid<bool> &usedDice)
{
	int row = 0, col = 0;
	while (true) {
		usedDice.setAt(row, col, false);
		if (!AdjacentPoint(usedDice.numRows(), usedDice.numCols(), row, col)) break;
	}
}

/*
 * Function: OffBoard
 * ---------------------
 * Checks to see whether or not the row and col being called upon is within the boundaries of the grid.
 */
bool OffBoard(Grid<char> board, int row, int col)
{
	return (row < 0 || row >= board.numRows() || col < 0 || col >= board.numCols());
}

/*
 * FindPossiblePath
 * -------------------
 * Covers all the possible paths of the dice specified by boggle rules in a clockwise manner.
 */
void FindPossiblePath(int &newRow, int &newCol, int row, int col, int i)
{
	switch(i) {
        case 0: newRow = row - 1; newCol = col; break; //North
        case 1: newRow = row - 1; newCol = col + 1; break; //Northeast
        case 2: newRow = row; newCol = col + 1; break; //East
        case 3: newRow = row + 1; newCol = col + 1; break; //Southeast
        case 4: newRow = row + 1; newCol = col; break; //South
        case 5: newRow = row + 1; newCol = col - 1; break; //Southwest
        case 6: newRow = row; newCol = col - 1; break; //West
        case 7: newRow = row - 1; newCol = col - 1; break; //Northwest
    }
}

/*
 * UnhighlightBoard
 * --------------------
 * Unhighlights the graphic display of the boggle board. Ensures the highlighted word is 
 * returned to normal.
 */
void UnhighlightBoard(Grid<char> boggleBoard)
{
	int row = 0, col = 0;
	Pause(pauseTime); //creates highlighting animation of word
	while (true) {
		HighlightCube(row, col, false);
		if (!AdjacentPoint(boggleBoard.numRows(), boggleBoard.numCols(), row, col)) break;
	}
}

/*
 * Function: RandomizeResponse
 * -----------------------------
 * Returns a random string from the list of const repsonses to bad user guesses.
 */
string RandomizeResponse()
{
	return BadUserGuessResponse[RandomInteger(0, 7)];
}

/*
 * Function: CheckUserGuess
 * ---------------------------
 * Checks if the format of user guess meets all requirements of the boggle rules.
 *
 *@return string userGuess when function has confirmed that all requirements have been met.
 */
string CheckUserGuess(Lexicon wordList, Lexicon usedWords)
{
	while (true) { //prompts until appropriate guess is received
		cout << "Enter a word: ";
		string userGuess = ConvertToUpperCase(GetLine());
		if (userGuess == "") return "";
		if (userGuess.length() < minWordLength) { //checks if minimum word length is met
			cout << "I'm sorry, but we have our standards." << endl;
			cout << "That word doesn't meet the minimum word length of 4." << endl;
			PlayNamedSound("whoops.wav");
		} else if (!wordList.containsWord(userGuess)) { //checks if the guess is actually a word
			cout << "That's not a word! " << RandomizeResponse() << endl;
			PlayNamedSound("whoops.wav");
		} else if (usedWords.containsWord(userGuess)) { //checks if the guess has already been used
			cout << "You've already guessed that! " << RandomizeResponse() << endl;
			PlayNamedSound("whoops.wav");
		} else {
			return userGuess;
		}
	}
}

/**
 * Function: FindUserGuessOnBoard
 * ---------------------------------
 * Recursively checks if user guess can be created from the dice configuration of boggleBoard starting from the specified
 * location.
 *
 *@return bool	true if the user guess has been created from boggleBoard.
 *				false if all possibilities have been exhausted and user guess has not been created.
 */
bool FindUserGuessOnBoard(Grid<char> boggleBoard, int row, int col, string partialWord, string userGuess, Grid<bool> usedDice)
{
	int newRow, newCol;
	if (OffBoard(boggleBoard, row, col) || usedDice.getAt(row, col) == true) return false; 
	partialWord += boggleBoard.getAt(row, col); //keeps track of the current char arrangement
	if (userGuess.substr(0, partialWord.length()) != partialWord) return false; //checks if partialWord is a prefix of the user guess
	usedDice.setAt(row, col, true); //marks the dice as used
	if (partialWord == userGuess) { //checks if user guess has been successfully created
		HighlightCube(row, col, true);
		return true;
	}
	for(int i = 0; i < 8; i++) { //there are 8 possible paths the char arrangement can take
		FindPossiblePath(newRow, newCol, row, col, i);
		if (FindUserGuessOnBoard(boggleBoard, newRow, newCol, partialWord, userGuess, usedDice)) {
			HighlightCube(row, col, true);
			return true;
		}
	}
	usedDice.setAt(row, col, false); //unmarks the dice as unused when current char configuration turns out fruitless
	return false;
}

/*
 * Function: InputGuesses
 * -------------------------
 * Allows the user to input guesses. Runs checks on user guess to see if it meets all boggle requirements.
 */
void InputGuesses(Grid<char> boggleBoard, Lexicon wordList, Lexicon &usedWords, Grid<bool> usedDice)
{	
	cout << endl << "Ok, take all the time you want and find all the words you can!" << endl;
	cout << "Signal that you're finished by entering an empty line." << endl << endl;
	while (true) {
		int row= 0, col = 0;
		string userGuess = CheckUserGuess(wordList, usedWords); //checks if format rules are met
		if (userGuess == "") break; //checks if sentinel of user hitting return has been occured
		while (true) {
			if (FindUserGuessOnBoard(boggleBoard, row, col, "", userGuess, usedDice)) { 
                //checks if user guess can be created from boggleBoard
				usedWords.add(userGuess); //records user guess in lexicon of used words
				RecordWordForPlayer(userGuess, Human);
				UnhighlightBoard(boggleBoard);
				PlayNamedSound("excellent.wav");
				break;
			}
			if (!AdjacentPoint(boggleBoard.numRows(), boggleBoard.numCols(), row, col)) { 
                //checks if end of board has been reached without finding appropriate dice config
				cout << "You can't make that word! " << RandomizeResponse() << endl;
				PlayNamedSound("whoops.wav");
				break;
			}
		}
	}
}

/*
 * Function: FindWordsonBoard
 * ------------------------------
 * Recursively searchs for all possible words that can be created from dice configuration of boggleBoard starting from
 * the specified location.
 *
 *@return bool	true if the char arrangement found is an actual word.
 *				false if all possibilities have been exhausted and no word is created.
 */
bool FindWordsOnBoard(Grid<char> boggleBoard, int row, int col, string partialWord, Grid<bool> usedDice, Lexicon wordList, Lexicon &usedWords)
{	
	int newRow, newCol;
	if (OffBoard(boggleBoard, row, col) || usedDice.getAt(row, col) == true) return false; 
	partialWord += boggleBoard.getAt(row, col); //keeps track of current char arrangement
	if (!wordList.containsPrefix(partialWord)) return false; //checks if partialWord is a prefix of any word in wordList
	usedDice.setAt(row, col, true); //marks the dice as used
	if (wordList.containsWord(partialWord) && partialWord.length() >= minWordLength && !usedWords.containsWord(partialWord)) {
        //checks if partialWord is an actual word meeting the minimum length and has not been used by either the player or computer
		RecordWordForPlayer(partialWord, Computer);
		usedWords.add(partialWord); //adds the word found to list of used words
	}
	for(int i = 0; i < 8; i++) { //there are 8 possible paths the char arrangement can take
		FindPossiblePath(newRow, newCol, row, col, i);
		if (FindWordsOnBoard(boggleBoard, newRow, newCol, partialWord, usedDice, wordList, usedWords)) return true;
	}
	usedDice.setAt(row, col, false); //unmarks the dice as unused when current char configuration turns out fruitless
	return false;
}

/*
 * Function: FindRemainingWords
 * ------------------------------
 * Goes through all of the dice on boggleBoard, calling FindWordsOnBoard to search for all remaining
 * words that be created from the dice configuration of boggleBoard starting from the specific dice location.
 */
void FindRemainingWords(Grid<char> boggleBoard, Lexicon wordList, Lexicon &usedWords, Grid<bool> usedDice)
{
	int row = 0, col = 0;
	while (true) {
		FindWordsOnBoard(boggleBoard, row, col, "", usedDice, wordList, usedWords);
		if (!AdjacentPoint(boggleBoard.numRows(), boggleBoard.numCols(), row, col)) break;
	}
}

/*
 * Function: GameContinue
 * ------------------------
 * Provides user with option to play another round of boggle with the computer.
 */
bool GameContinue()
{
	cout << "Would you like to play again? ";
	string answer = VerifyUserInput(ConvertToUpperCase(GetLine()));
	if (answer == "NO") return false;
	return true;
}

/*
 * Function: main
 * -----------------
 * Serves as entry point of program. Takes in all user inputs to determine specific boggle configuration.
 * Then, it gives the user a chance to find words in the boggleBoard. Then, the computer takes over
 * to find any remaining words. Finally, gives user option to play another round.
 *
 *@return 0 if program completed successfully.
 */
int main()
{
	Randomize(); //initializes random constructor
	SetWindowSize(8, 5);
	InitGraphics();
	Welcome();
	GiveInstructions();
	Lexicon wordList("lexicon.dat"); //generates list of all possible words
	while (true) {
		Lexicon usedWords; //generates a list that stores all words found by the player and computer
		InitGraphics();
		SoundFeature();
		int boardDimension = BoggleBoardSize();
		DrawBoard(boardDimension, boardDimension);
		Grid<char> boggleBoard(boardDimension, boardDimension);
		if (!UserBoardConfiguration()) {
			InitializeRandomBoard(boggleBoard); //if user chooses not to specify board configuration, a random one is generated
		} else {
			string diceConfig = GetDiceConfiguration(boardDimension);
			SetDiceConfiguration(boggleBoard, diceConfig);
		}
		DrawBoggleBoard(boggleBoard);
		Grid<bool> usedDice(boggleBoard.numRows(), boggleBoard.numCols());
		CreateMarker(usedDice);
		InputGuesses(boggleBoard, wordList, usedWords, usedDice); //player's turn
		FindRemainingWords(boggleBoard, wordList, usedWords, usedDice); //computer's turn
		PlayNamedSound("thats pathetic.wav"); //assumes the player will always lose to the computer
		if (!GameContinue()) break;
	}
	return 0;
}