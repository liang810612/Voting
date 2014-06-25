// --------------------------
// projects/collatz/Collatz.h
// Copyright (C) 2014
// Glenn P. Downing
// --------------------------

#ifndef Voting_h
#define Voting_h

// --------
// includes
// --------

#include <iostream> // istream, ostream
#include <utility>  // pair
#include <string>
#include <vector>



// ------------
// voting_read
// ------------

/**
 * read two ints
 * @param  r a  std::istream
 * @return a pair of the two ints, otherwise a pair of zeros
 */
bool voting_read (std::istream&);


// -------------
// voting_print
// -------------

/**
 * print three ints
 * @param w a std::ostream
 * @param i the beginning of the range, inclusive
 * @param j the end       of the range, inclusive
 * @param v the max cycle length
 */
void voting_print (std::ostream&, int, int, int);

// -------------
// voting_eval
// -------------

std::vector<std::string> voting_eval(int [1000][20], std::string [20], int, int);

// -------------
// voting_solve
// -------------

/**
 * read, eval, print loop
 * @param r a std::istream
 * @param w a std::ostream
 */
void voting_solve (std::istream&, std::ostream&);

#endif // Voting_h
