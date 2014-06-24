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

int ballot[1000][20];
string candidName[20];
int candidTotalNum;
int ballotNum;
int caseNum;

// ------------
// voting_read
// ------------

bool voting_read (std::istream& r) {
    //char split_char = "\n";

    string line;
    istringstream iss;
    caseNum = 0;;
    candidTotalNum = 0;
    ballotNum = 0;
    int candidNum = 0;

    bool caseStart = false;
    bool blank = false;
    bool candidNumCheck = false;

    ballot[1000][20] = {};
    candidName[20] = {};

    int c = 0; //index for candidName array



    while ( getline(r, line)){
        iss.clear();
        iss.str(line);

    

        if(!caseStart){
            caseNum = atoi(line.c_str());
            caseStart = true;
        }
        else{
            //check for blank line

            if(line.empty()){

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
                else{// check and put tokens into specific array
                	if(isalpha(line[0])){
                		//It's checking candidate's name
                		candidName[c] = line;
                		c++;
                	}
                	else{
                		//It's checking ballot

	                    string token;
	                    stringstream s(line);
	                    while(getline(s, token, ' ')){

	                        ballot[ballotNum][candidNum] = atoi(token.c_str());

	                        //cout << "ballot list: " << ballot[ballotNum][candidNum] << endl;
	                        candidNum++;
	                    }
	                    ballotNum++;
	                    candidNum  = 0;
                	}
                }// end of checking token array
                
            }

        }// end for caseStart else

    }
	/*
    for(int k = 0; k < c; k++){
    	cout << candidName[k] << endl;
    }
    */
    /*
    for(int i = 0; i < ballotNum; i++){

        for(int j = 0; j < candidTotalNum; j++){
            cout << ballot[i][j] << " ";
        }
        cout << endl;
    }
    */

//*************************TODO******************************
    //need to make ballotNum = 0 for next case


    return true; 
}
	

// -------------
// voting_print
// -------------


//void voting_print (std::ostream& w, int i, int j, int v) {
    //w << i << " " << j << " " << v << std::endl;}


// -------------
// voting_eval
// -------------

vector<string> voting_eval(){
	vector<string> resultName;
	vector<int> loser;
	vector<int> count; //the ballot counter

	cout << "candidTotalNum: " << candidTotalNum << endl;
	cout << "ballotNum: " << ballotNum<< endl;
	cout << "caseNum: " << caseNum << endl;

	/*//no cache ver
	for(int i = 0; i < candidTotalNum; i++){
		for(int j = 0; j < ballotNum; j++){

			//skip index 0 for easy readible reason
			count[ballot[i][j]]++;
		}

		//if there is a winner with more than half votes and most votes 
		auto it = max_element(std::begin(count), std::end(count));
		if(it > (ballotNum / 2)){
			resultName.push_back()
		}


	}*/
	return resultName;



}

// -------------
// voting_solve
// -------------

void voting_solve (std::istream& r, std::ostream& w) {
    while (true) {
        bool check = voting_read(r);
        vector<string> winner = voting_eval();
   		//cout << winner << endl;
        //voting_print(w, p.first, p.second, v);
    }
}
