/*
 * File: simpio.h
 * Last modified on Wed Jul 22 07:01:03 2009 by eroberts
 *      modified on Wed Sep 18 13:34:29 2002 by zelenski
 * -----------------------------------------------------
 * This interface provides access to a simple package of
 * functions that simplify the reading of console input.
 */

#ifndef _simpio_h
#define _simpio_h

#include "genlib.h"

/*
 * Function: GetInteger
 * Usage: n = GetInteger();
 * ------------------------
 * GetInteger reads a line of text from standard input and scans
 * it as an integer.  The integer value is returned.  If an
 * integer cannot be scanned or if more characters follow the
 * number, the user is given a chance to retry.
 */

int GetInteger();

/*
 * Function: GetLong
 * Usage: n = GetLong();
 * ---------------------
 * GetLong reads a line of text from standard input and scans
 * it into a long integer.  The long is returned.  If the
 * number cannot be scanned or if extra characters follow it,
 * the user is given a chance to retry.
 */

long GetLong();

/*
 * Function: GetReal
 * Usage: x = GetReal();
 * ---------------------
 * GetReal reads a line of text from standard input and scans
 * it as a double.  If the number cannot be scanned or if extra
 * characters follow after the number ends, the user is given
 * a chance to reenter the value.
 */

double GetReal();

/*
 * Function: GetLine
 * Usage: s = GetLine();
 * ---------------------
 * GetLine reads a line of text from standard input and returns
 * the line as a string.  The newline character that terminates
 * the input is not stored as part of the string that is returned.
 */

string GetLine();

/*
 * Function: OpenFile
 * Usage: OpenFile(infile, filename);
 *        OpenFile(outfile, filename);
 * -----------------------------------
 * Opens the specified file.  The direction is indicated by the type
 * of file (ifstream or ofstream) supplied as the first argument.
 * Any failure is detected within the implementation and is reported
 * using the Error function.
 */

void OpenFile(ifstream & infile, string filename);
void OpenFile(ofstream & outfile, string filename);

/*
 * Function: AskUserForInputFile
 * Usage: AskUserForInputFile(infile);
 *        AskUserForInputFile(prompt, infile);
 * -------------------------------------------
 * Opens a text file whose name is entered by the user.  If the file
 * does not exist, the user is given additional chances to enter a
 * valid file.  The prompt string in the second form of the function
 * is used to tell the user what kind of file is required.
 */

void AskUserForInputFile(ifstream & infile);
void AskUserForInputFile(string prompt, ifstream & infile);

#endif
