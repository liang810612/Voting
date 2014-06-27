// --------------------------------
// projects/Voting/TestVoting.c++
// Copyright (C) 2014
// Terry Liang, Sharon Chung
// --------------------------------

/*
Google Test Libraries:
    % ls -al /usr/include/gtest/
    ...
    gtest.h
    ...

    % locate libgtest.a
    /usr/lib/libgtest.a

    % locate libpthread.a
    /usr/lib/x86_64-linux-gnu/libpthread.a
    /usr/lib32/libpthread.a

    % locate libgtest_main.a
    /usr/lib/libgtest_main.a

To compile the test:
    % g++-4.7 -fprofile-arcs -ftest-coverage -pedantic -std=c++11 -Wall Voting.c++ TestVoting.c++ -o TestVoting -lgtest -lgtest_main -lpthread

To run the test:
    % valgrind Voting

To obtain coverage of the test:
    % gcov-4.7 -b Voting.c++ TestVoting.c++
*/


/*
To test the cppunit program:
    % g++ -pedantic -std=c++0x -Wall Voting.c++ TestVoting.c++ -o TestVoting -lcppunit -ldl
    ...
    % valgrind TestVoting >& TestVoting.c++.out
*/   

// --------
// includes
// --------

#include <iostream> // cout, endl
#include <sstream>  // istringtstream, ostringstream
#include <string>   // ==
#include <utility>  // make_pair, pair
#include <vector>   //vector

#include "gtest/gtest.h"

#include "cppunit/extensions/HelperMacros.h" // CPPUNIT_TEST, CPPUNIT_TEST_SUITE, CPPUNIT_TEST_SUITE_END
#include "cppunit/TestFixture.h"             // TestFixture
#include "cppunit/TextTestRunner.h"          // TextTestRunner


#include "Voting.h"

using namespace std;



// -----------
// TestVoting
// -----------


struct TestVoting : CppUnit::TestFixture {


// ----
// read
// ----
void test_read_1() {

    std::istringstream r("3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2\n");
    vector<string> a = voting_read(r);
    vector<string> b = {"John Doe", "Jane Smith", "Sirhan Sirhan"};
    CPPUNIT_ASSERT(b[0] == "John Doe");
}


void test_read_2() {

    std::istringstream r("4\nRed\nGreen\nBlue\nOrange\n1 2 3 4\n2 1 3 4\n2 3 1 4\n1 2 3 4\n3 4 1 2\n4 3 2 1");
    vector<string> a = voting_read(r);
    vector<string> b = {"Red", "Green", "Blue", "Orange"};    
    CPPUNIT_ASSERT(b[0] == "Red");
}

void test_read_3() {

    std::istringstream r("2\nABC\nDEF\n1 2\n2 1\n1 2\n2 1");
    vector<string> a = voting_read(r);
    vector<string> b = {"ABC", "DEF"};    
    CPPUNIT_ASSERT(b[0] == "ABC");
}


// ----
// eval
// ----

void test_eval_1(){

    int ballot[1000][20] = { {1,2,3}, {2,1,3}, {2,3,1}, {1,2,3}, {3,1,2}};
    string candidName[20] = {"John Doe", "Jane Smith", "Sirhan Sirhan"};
    int candidTotalNum = 20; 
    int ballotNum = 5;

    vector<string> winners = voting_eval(ballot,candidName,candidTotalNum,ballotNum);
    CPPUNIT_ASSERT(winners.size() == 1);
    CPPUNIT_ASSERT(winners[0] == "John Doe");
}

void test_eval_2(){

    int ballot[1000][20] = { {1,2,3,4}, {2,1,3,4}, {2,3,1,4}, {1,2,3,4}, {3,4,1,2}, {4,3,2,1}};
    string candidName[20] = {"Red", "Green", "Blue", "Orange"};
    int candidTotalNum = 4; 
    int ballotNum = 6;

    vector<string> winners = voting_eval(ballot,candidName,candidTotalNum,ballotNum);
    CPPUNIT_ASSERT(winners.size() == 2);
    CPPUNIT_ASSERT(winners[0] == "Red");
    CPPUNIT_ASSERT(winners[1] == "Green");
}

void test_eval_3(){

    int ballot[1000][20] = { {1,2}, {2,1}, {1,2}, {2,1}};
    string candidName[20] = {"ABC", "DEF"};
    int candidTotalNum = 2; 
    int ballotNum = 4;

    vector<string> winners = voting_eval(ballot,candidName,candidTotalNum,ballotNum);
    CPPUNIT_ASSERT(winners.size() == 2);
    CPPUNIT_ASSERT(winners[0] == "ABC");
    CPPUNIT_ASSERT(winners[1] == "DEF");
}
   


// -----
// print
// -----
void test_print_1 () {

    std::ostringstream r;
    vector<string> result = {"John Doe"};
    voting_print(r, result);
    CPPUNIT_ASSERT(result[0] == "John Doe");

}

void test_print_2 () {

    std::ostringstream r;
    vector<string> result = {"Red", "Green"};
    voting_print(r, result);
    CPPUNIT_ASSERT(result[0] == "Red");

}

void test_print_3 () {

    std::ostringstream r;
    vector<string> result = {"ABC", "DEF"};
    voting_print(r, result);
    CPPUNIT_ASSERT(result[0] == "ABC");

}


   
// -----
// solve
// -----

void test_solve_1 () {

    std::istringstream r("1\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2");
    std::ostringstream w;
    voting_solve(r, w);

    CPPUNIT_ASSERT(r.str() == "1\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2");

}

void test_solve_2 () {
    std::istringstream r("1\n\n4\nRed\nGreen\nBlue\nOrange\n1 2 3 4\n2 1 3 4\n2 3 1 4\n1 2 3 4\n3 4 1 2\n4 3 2 1");
    std::ostringstream w;
    voting_solve(r, w);
    CPPUNIT_ASSERT(r.str() == "1\n\n4\nRed\nGreen\nBlue\nOrange\n1 2 3 4\n2 1 3 4\n2 3 1 4\n1 2 3 4\n3 4 1 2\n4 3 2 1");
}

void test_solve_3 () {
    std::istringstream r("1\n\n2\nABC\nDEF\n1 2\n2 1\n1 2\n2 1");
    std::ostringstream w;
    voting_solve(r, w);
    CPPUNIT_ASSERT(r.str() == "1\n\n2\nABC\nDEF\n1 2\n2 1\n1 2\n2 1");
}
    


// -----
// suite
// -----

CPPUNIT_TEST_SUITE(TestVoting);

CPPUNIT_TEST(test_read_1);
CPPUNIT_TEST(test_read_2);
CPPUNIT_TEST(test_read_3);
CPPUNIT_TEST(test_eval_1);
CPPUNIT_TEST(test_eval_2);
CPPUNIT_TEST(test_eval_3);
CPPUNIT_TEST(test_solve_1);
CPPUNIT_TEST(test_solve_2);
CPPUNIT_TEST(test_solve_3);
CPPUNIT_TEST(test_print_1);
CPPUNIT_TEST(test_print_2);
CPPUNIT_TEST(test_print_3);

CPPUNIT_TEST_SUITE_END();
};




int main(){
    using namespace std;
    // ios_base::sync_with_stdio(false);  // turn off synchronization with C I/O
    cout << "Testvoting.c++" << endl;

    CppUnit::TextTestRunner tr;
    tr.addTest(TestVoting::suite());
    tr.run();

    cout << "Done." << endl;
    return 0;}    

