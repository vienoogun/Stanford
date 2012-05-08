/*
 * File: genlib.h
 * Last modified on Wed Apr  1 13:17:23 2009 by eroberts
 *      modified on Wed Sep 18 13:41:31 2002 by zelenski
 * -----------------------------------------------------
 * This header file is indicated to be included in
 * all the programs written for CS106B/X and provides a few
 * common definitions. Note this header has a "using namespace std"
 * clause. If a file includes this header, it can then use
 * features from the std namespace without qualifying by scope.
 */

#ifndef _genlib_h
#define _genlib_h

/* This strange-looking pragma is here to disable a warning from Visual C++
 * about truncating long identifiers for debugging symbols. The warning is
 * harmless, but a little disconcernting, so we suppress it. It comes up
 * using STL and other long template expansions.
 */
#if defined(_MSC_VER)
#pragma warning(disable: 4786)
#endif

#include <string>
using namespace std;

/*
 * Function: Error
 * Usage: Error(msg)
 * ------------------
 * Error outputs an error string to the cerr stream and
 * then exits the program with a status code indicating failure.
 */

void Error(string str);

/*
 * Function macro: main
 * --------------------
 * The purpose of this macro definition is to rename the student
 * main to Main in order to allow a custom main defined in our
 * libraries to configure the application before passing control
 * back to the student program.
 */

#ifdef __APPLE__
#define main Main
#endif

#endif
