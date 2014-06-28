// --------------------------------
// projects/Voting/TestVoting.c++
// Copyright (C) 2014
// Terry Liang, Sharon Chung
// --------------------------------



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

    std::istringstream r("1\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2\n");
    vector<string> a = voting_read(r);
    int ballot[1000][20] = {{1,2,3},{2,1,3},{2,3,1},{1,2,3},{3,1,2}};
    string candidName[20] = {"John Doe","Jane Smith", "Sirhan Sirhan", };
    int candidTotalNum = 3;
    int ballotNum = 5;
    vector<string> result = voting_eval(ballot, candidName, candidTotalNum, ballotNum); 
    CPPUNIT_ASSERT(result[0] == "John Doe");
}


void test_read_2() {

    std::istringstream r("1\n\n4\nRed\nGreen\nBlue\nOrange\n1 2 3 4\n2 1 3 4\n2 3 1 4\n1 2 3 4\n3 4 1 2\n4 3 2 1");
    vector<string> a = voting_read(r); 
    int ballot[1000][20] = {{1,2,3,4},{2,1,3,4},{2,3,1,4},{1,2,3,4},{3,4,1,2},{4,3,2,1}};
    string candidName[20] = {"Red","Green", "Blue", "Orange"};
    int candidTotalNum = 4;
    int ballotNum = 6;
    vector<string> result = voting_eval(ballot, candidName, candidTotalNum, ballotNum); 
    CPPUNIT_ASSERT(result[0] == "Red");
    CPPUNIT_ASSERT(result[1] == "Green");
}

void test_read_3() {

    std::istringstream r("1\n\n2\nABC\nDEF\n1 2\n2 1\n1 2\n2 1");
    int ballot[1000][20] = {{1,2},{2,1},{1,2},{2,1}};
    string candidName[20] = {"ABC","DEF"};
    int candidTotalNum = 2;
    int ballotNum = 4;
    vector<string> result = voting_eval(ballot, candidName, candidTotalNum, ballotNum); 
    CPPUNIT_ASSERT(result[0] == "ABC");
}


// ----
// eval
// ----

void test_eval_1(){

    std::istringstream r("1\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2");
    vector<string> a = voting_read(r);
    int ballot[1000][20] = { {1,2,3}, {2,1,3}, {2,3,1}, {1,2,3}, {3,1,2}};
    string candidName[20] = {"John Doe", "Jane Smith", "Sirhan Sirhan"};
    int candidTotalNum = 20; 
    int ballotNum = 5;


    vector<string> winners = voting_eval(ballot,candidName,candidTotalNum,ballotNum);
    CPPUNIT_ASSERT(winners.size() == 1);
    CPPUNIT_ASSERT(winners[0] == "John Doe");
}

void test_eval_2(){

    int ballot[1000][20] = {{1,2,3,4}, {2,1,3,4}, {2,3,1,4}, {1,2,3,4}, {3,4,1,2}, {4,3,2,1}};
    string candidName[20] = {"Red", "Green", "Blue", "Orange"};
    int candidTotalNum = 4; 
    int ballotNum = 6;

    vector<string> winners = voting_eval(ballot,candidName,candidTotalNum,ballotNum);
    CPPUNIT_ASSERT(winners.size() == 2);
    CPPUNIT_ASSERT(winners[0] == "Red");
    CPPUNIT_ASSERT(winners[1] == "Green");
}

void test_eval_3(){

    int ballot[1000][20] = {{1,2}, {2,1}, {1,2}, {2,1}};
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

    std::istringstream r("1\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2");
    std::ostringstream w;
    voting_solve(r, w);  
    vector<string> result = voting_read(r);
    voting_print(w, result);
    CPPUNIT_ASSERT(w.str() == "John Doe\n");
}

void test_print_2 () {

    std::istringstream r("1\n\n4\nRed\nGreen\nBlue\nOrange\n1 2 3 4\n2 1 3 4\n2 3 1 4\n1 2 3 4\n3 4 1 2\n4 3 2 1");
    std::ostringstream w;
    voting_solve(r, w);
    vector<string> result = voting_read(r);
    voting_print(w, result);
    CPPUNIT_ASSERT(w.str() == "Red\nGreen\n");
 }

void test_print_3 () {

    std::istringstream r("1\n\n2\nABC\nDEF\n1 2\n2 1\n1 2\n2 1");
    std::ostringstream w;
    voting_solve(r, w);
    vector<string> result = voting_read(r);
    voting_print(w, result);
    CPPUNIT_ASSERT(w.str() == "ABC\nDEF\n");
}


   
// -----
// solve
// -----

void test_solve_1 () {

    std::istringstream r("1\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2");
    std::ostringstream w;
    voting_solve(r, w);
    vector<string> result = voting_read(r);
    voting_print(w, result);
    CPPUNIT_ASSERT(r.str() =="1\n\n3\nJohn Doe\nJane Smith\nSirhan Sirhan\n1 2 3\n2 1 3\n2 3 1\n1 2 3\n3 1 2");
    CPPUNIT_ASSERT(w.str() == "John Doe\n");
}

void test_solve_2 () {

    std::istringstream r("1\n\n4\nRed\nGreen\nBlue\nOrange\n1 2 3 4\n2 1 3 4\n2 3 1 4\n1 2 3 4\n3 4 1 2\n4 3 2 1");
    std::ostringstream w;
    voting_solve(r, w);
    vector<string> result = voting_read(r);
    voting_print(w, result);
    CPPUNIT_ASSERT(r.str() =="1\n\n4\nRed\nGreen\nBlue\nOrange\n1 2 3 4\n2 1 3 4\n2 3 1 4\n1 2 3 4\n3 4 1 2\n4 3 2 1");
    CPPUNIT_ASSERT(w.str() == "Red\nGreen\n");
}

void test_solve_3 () {

    std::istringstream r("1\n\n2\nABC\nDEF\n1 2\n2 1\n1 2\n2 1");
    std::ostringstream w;
    voting_solve(r, w);
    vector<string> result = voting_read(r);
    voting_print(w, result);
    CPPUNIT_ASSERT(r.str() =="1\n\n2\nABC\nDEF\n1 2\n2 1\n1 2\n2 1");
    CPPUNIT_ASSERT(w.str() == "ABC\nDEF\n");
}

// -----
// voting_arrange_loser_vote
// -----

void test_voting_arrange_loser_vote_1(){
    int ballot[1000][20] = { {1,2,3}, {2,1,3}, {2,3,1}, {1,2,3}, {3,1,2}};
    string names [3] = {"John Doe", "Jane Smith", "Sirhan Sirhan"};
    int votes[3] = {2,2,1};
    vector<Candidate> candidateList;
    for(int i = 0; i < 3 ; i ++){
        struct Candidate c;

        c.name = names[i];
        c.numVote = votes[i];
        c.isLoser = false; 

        candidateList.push_back(c);
    }

   CPPUNIT_ASSERT(candidateList[2] == false);
}

void test_voting_arrange_loser_vote_2(){
    int ballot[1000][20] = {{1,2,3,4}, {2,1,3,4}, {2,3,1,4}, {1,2,3,4}, {3,4,1,2}, {4,3,2,1}};
    string names [4] = {"Red", "Green", "Blue", "Orange"};
    int votes[4] = {2,2,1,1};
    vector<Candidate> candidateList;
    for(int i = 0; i < 4 ; i ++){
        struct Candidate c;

        c.name = names[i];
        c.numVote = votes[i];
        c.isLoser = false; 

        candidateList.push_back(c);
    }
    CPPUNIT_ASSERT(candidateList[0] == false);
    CPPUNIT_ASSERT(candidateList[1] == false);
}

void test_voting_arrange_loser_vote_3(){
    int ballot[1000][20] = {{1,2},{2,1},{1,2},{2,1}};
    string names [2] = {"ABC", "DEF"};
    int votes[2] = {2,2};
    vector<Candidate> candidateList;
    for(int i = 0; i < 4 ; i ++){
        struct Candidate c;

        c.name = names[i];
        c.numVote = votes[i];
        c.isLoser = false; 

        candidateList.push_back(c);
    }

   CPPUNIT_ASSERT(candidateList[0] == false);
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
CPPUNIT_TEST(test_voting_arrange_loser_vote_1);
CPPUNIT_TEST(test_voting_arrange_loser_vote_2);
CPPUNIT_TEST(test_voting_arrange_loser_vote_3);

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

