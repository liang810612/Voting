// ----------------------------
// projects/collatz/Collatz.c++
// Copyright (C) 2014
// Glenn P. Downing
// ----------------------------

// --------
// includes
// --------
//#include <boost/algorithm/string.hpp>
#include <cassert>  // assert
#include <cstring>
#include <iostream> // endl, istream, ostream
#include <utility>  // make_pair, pair
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <vector>

#include "Voting.h"
using namespace std;

// ------------
// voting_read
// ------------

bool voting_read (std::istream& r) {
    //char split_char = "\n";

    string line;
    istringstream iss;
    int caseNum = 0;;
    int candidTotalNum = 0;
    int ballotNum = 0;
    int candidNum = 0;

    bool caseStart = false;
    bool blank = false;
    bool candidNumCheck = false;
    int ballot[1000][20] = {};



    while ( getline(r, line)){
        iss.clear();
        iss.str(line);

        
        cout << line << endl;

        if(!caseStart){
            caseNum = atoi(line.c_str());
            caseStart = true;
        }
        else{
            //check for blank line
            if(line[0] == '\n'){
                if(!blank){
                    //case just started
                    blank = true;
                    candidNumCheck = true;
                }
                else{
                    //end of case
                    blank = false;
                }
            }
            else{
                
                if(candidNumCheck){ 
                    //if it is on the candidate number line
                    candidTotalNum = atoi(line.c_str());
                    candidNumCheck = false; 
                }
                else{
                    //It is checking ballot
                    string token;
                    stringstream s(line);
                    while(getline(s, token, ' ')){
                        ballot[ballotNum][candidNum] = atoi(token.c_str());
                        candidNum++;
                    }
                    ballotNum++;
                }
                
            }

        }
        


        


    }

/*
    string temp = strtok(iss, "\n");
    while (temp != NULL){
        cout << temp << endl;
        temp = strtok(NULL, "\n");
    }
    */


    //std::cout << caseNum << std::endl;
    return true; 
}
	

// -------------
// collatz_print
// -------------

/*
void collatz_print (std::ostream& w, int i, int j, int v) {
    w << i << " " << j << " " << v << std::endl;}
*/
// -------------
// collatz_solve
// -------------

void voting_solve (std::istream& r, std::ostream& w) {
    while (true) {
        bool p = voting_read(r);
   
        //collatz_print(w, p.first, p.second, v);
    }
}
