/*
 * File: lexicon.h
 * Last modified on Fri Jun 19 17:43:00 2009 by eroberts
 *      modified on Tue Oct 10 20:47:22 2006 by zelenski
 * -----------------------------------------------------
 * Defines the Lexicon class.
 */

#ifndef _lexicon_h
#define _lexicon_h

#include "genlib.h"
#include "foreach.h"
#include "set.h"
#include "stack.h"

/*
 * Class: Lexicon
 * --------------
 * This class is used to represent a lexicon, or word list. The main
 * difference between the lexicon abstraction and a map or dictionary
 * container is that the lexicon does not provide any mechanism for
 * storing definitions of words; the lexicon contains only words, with
 * no associated information.  The lexicon supports very efficient
 * lookup operations for words and prefixes.  You can populate the
 * lexicon by adding words one at a time or by constructing a lexicon
 * with words read from a file. Here is sample use of a Lexicon object:
 *
 *      Lexicon lex("lexicon.dat");
 *      lex.add("doughnut");
 *      if (lex.containsPrefix("fru") || lex.containsWord("ball")) . . .
 */

#include <cctype>

class Lexicon {

public:

/* Forward references */
	class Iterator;

/*
 * Constructor: Lexicon
 * Usage: Lexicon lex;
 * Lexicon *lp = new Lexicon;
 * --------------------------
 * The constructor initializes a new empty lexicon.
 */
	Lexicon();

/*
 * Constructor: Lexicon
 * Usage: Lexicon lex("lexicon.dat");
 * Lexicon *lp = new Lexicon("lexicon.txt");
 * -----------------------------------------
 * The constructor initializes a new lexicon which is populated with
 * the words read from the specified file.  The file is expected to
 * be either a special special binary format that represents a saved
 * lexicon or a plain text file of words, one word per line. The
 * constructor reads the file and adds all of its words to this lexicon.
 * The file must be in the same folder as the program to be found. If
 * the file doesn't exist or is malformed, Error is called to exit the program.
 */
	Lexicon(string filename);

/*
 * Destructor: ~Lexicon
 * Usage: delete lp;
 * ------------------
 * The destructor deallocates any storage associated with the lexicon.
 */
	~Lexicon();

/*
 * Method: size
 * Usage: count = lex.size();
 * --------------------------
 * This method returns the number of words contained in this lexicon.
 */
	int size();

/*
 * Method: isEmpty
 * Usage: if (lex.isEmpty()) . . .
 * -------------------------------
 * This method returns true if this lexicon contains no words, false otherwise.
 */
	bool isEmpty();

/*
 * Method: add
 * Usage: lex.add("computer");
 * ---------------------------
 * This method adds the specified word to this lexicon.
 */
	void add(string word);

/*
 * Method: addWordsFromFile
 * Usage: lex.addWordsFromFile("words.txt");
 * -----------------------------------------
 * This method reads the file and adds all of its words to this lexicon.
 * The file is expected to be either a special special binary format that
 * represents a saved lexicon or a plain text file of words, one word
 * per line.  The file must be in the same folder as the program to be
 * found.  If the file doesn't exist or is malformed, Error is called to
 * exit the program.
 */
	void addWordsFromFile(string filename);

/*
 * Method: containsWord
 * Usage: if (lex.containsWord("happy"))...
 * ----------------------------------------
 * This method returns true if word is contained in this lexicon,
 * false otherwise.  Words are considered case-insensitively, "zoo"
 * is the same as "ZOO" or "zoo".
 */
	bool containsWord(string word);

/*
 * Method: containsPrefix
 * Usage: lex.containsPrefix("mo");
 * --------------------------------
 * This method returns true if any words in this lexicon begin with
 * prefix, false otherwise.  A word is defined to be a prefix of itself
 * and the empty string is a prefix of everything. Prefixes are considered
 * case-insensitively, "mo" is a prefix of "MONKEY" or "Monday".
 */
	bool containsPrefix(string prefix);

/*
 * Method: clear
 * Usage: lex.clear();
 * -------------------
 * This method removes all words from this lexicon. The lexicon
 * will be empty after being cleared.
 */
	void clear();

/*
 * Method: iterator
 * Usage: iter = lexicon.iterator();
 * ---------------------------------
 * This method creates an iterator that allows the client to
 * iterate through the elements in this lexicon.  The elements are
 * returned in lexicographic order
 *
 * The idiomatic code for accessing elements using an iterator is
 * to create the iterator from the collection and then enter a loop
 * that calls next() while hasNext() is true, like this:
 *
 *     Lexicon::Iterator iter = lexicon.iterator();
 *     while (iter.hasNext()) {
 *         string word = iter.next();
 *         . . .
 *     }
 *
 * This pattern can be abbreviated to the following more readable form:
 *
 *     foreach (string word in lexicon) {
 *         . . .
 *     }
 *
 * To avoid exposing the details of the class, the definition of the
 * Iterator class itself appears in the private/lexicon.h file.
 */

	Iterator iterator();

/*
 * SPECIAL NOTE: mapping support
 * -----------------------------
 * The lexicon supports a mapping operation which allows the client to access
 * words one by one.  In general, mapping is intended for _viewing_ words
 * and can behave unpredictably if you attempt to modify the lexicon's contents
 * during mapping.
 */

/*
 * Method: mapAll
 * Usage: vector.mapAll(Print);
 * ----------------------------
 * This method iterates through the lexicon's contents
 * and calls the function fn once for each word.
 */
	void mapAll(void (*fn)(string word));

/*
 * Method: mapAll
 * Usage: lexicon.mapAll(PrintToFile, outputStream);
 * -------------------------------------------------
 * This method iterates through this lexicon and calls the function fn
 * once for each word, passing the word and the client's data. That
 * data can be of whatever type is needed for the client's callback.
 */
	template <typename ClientDataType>
	void mapAll(void (*fn)(string word, ClientDataType &),
	            ClientDataType & data);

private:

#include "private/lexicon.h"

};

#include "private/lexicon.cpp"

#endif
