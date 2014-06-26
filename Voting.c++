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

bool voting_read (std::istream& r, int ballot[1000][20], string candidName[20] , int cTN, int bN, int cN) {
    //char split_char = "\n";

    string line;
    istringstream iss;

    
    int candidNum = 0;
    int candidTotalNum = cTN;
    int ballotNum = bN;
    int caseNum = cN;

    bool caseStart = false;
    bool blank = false;
    bool candidNumCheck = false;

    vector<Candidate> CandidateList; // list of structs


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
	                        candidNum++;
	                    }
	                    ballotNum++;
	                    candidNum  = 0;
                	}
                }// end of checking token array
                
            }

        }// end for caseStart else

    }
	
    
    // for(int k = 0; k < c; k++){
    // 	cout << candidName[k] << endl;
    // }
    
    
    // for(int i = 0; i < ballotNum; i++){

    //     for(int j = 0; j < candidTotalNum; j++){
    //         cout << ballot[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    
    

//*************************TODO******************************
    //need to make ballotNum = 0 for next case

   //cout << candidNum << " ";






 voting_eval(ballot, candidName, candidTotalNum, ballotNum);

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


vector<Candidate> voting_eval(int ballot[1000][20], string candidName[20], int candidTotalNum, int ballotNum){
	

    vector<Candidate> winnerIndex;
    vector<Candidate> loserIndex;
    vector<Candidate> remainIndex; // For not 1st round only, all candidates whose numVote != 0


    int max = 0;
    int min = ballotNum;


    bool win = true;
    int TEMP = 0;

    bool result = false; 
    bool firstRound = true;
    int currentCol = 0;

    vector<Candidate> CandidateList;

    cout << candidTotalNum << endl;

    struct Candidate candidateList[candidTotalNum];



    //Initialize all candidates
    for(int i = 0; i < candidTotalNum; i++){
        
        candidateList[i].name = candidName[i];
        candidateList[i].numVote = 0;
        candidateList[i].isLoser = false; 
    }



    //count vote
    for(int j = 0; j < ballotNum; j++){
            int temp = ballot[j][currentCol] - 1;

            candidateList[temp].numVote++;

            //update the ballot # and ballot column ptr
            candidateList[temp].ballotChoice.push_back(make_pair(j, currentCol));


            //trying to find max winner
            if(candidateList[temp].numVote > max){
                winnerIndex.clear();
                max = candidateList[temp].numVote;
                winnerIndex.push_back(candidateList[temp]);
            }
            else if(candidateList[temp].numVote == max){
                winnerIndex.push_back(candidateList[temp]);
            }

            //trying to find min loser
            else if(candidateList[temp].numVote < min){
                loserIndex.clear();
                min = candidateList[temp].numVote;
                loserIndex.push_back(candidateList[temp]);
                candidateList[temp].isLoser = true;
            }
            else if(candidateList[temp].numVote == min){
                loserIndex.push_back(candidateList[temp]);
                candidateList[temp].isLoser = true;
            }

            
    }

    //Check for the candidate doesn't get any vote during 1st preference round
    //we can add bool to check for 1st time
    for(int i = 0; i < candidTotalNum; i++){
        if(candidateList[i].numVote == 0){
            loserIndex.push_back(candidateList[i]);
            candidateList[i].isLoser = true;
        }
    }

    //During 1st round, if everyone gets same amount of vote number
    if(max == min)
        return candidateList;


    while( !result ){

        int candidateCounter = 0; //only for not 1st round usage

        if( ! firstRound ){
            //get max and min
            remainIndex.clear();

            for(int i = 0; i < candidTotalNum; i++){

                remainIndex.push_back(candidateList[i]);

                if(candidateList[i].numVote != 0)
                    candidateCounter++;
                
                //trying to find max winner
                if(candidateList[i].numVote > max){
                    winnerIndex.clear();
                    max = candidateList[i].numVote;
                    winnerIndex.push_back(candidateList[i]);
                }
                else if(candidateList[i].numVote == max){
                    winnerIndex.push_back(candidateList[i]);
                }


                //trying to find min loser
                // it's possible the candidate is a winner but didn't get any vote at second round, we are trying to find new losers
                else if(candidateList[i].numVote < min && candidateList[i].numVote != 0){
                    loserIndex.clear();
                    min = candidateList[i].numVote;
                    loserIndex.push_back(candidateList[i]);
                    candidateList[i].isLoser = true;
                }
                else if(candidateList[i].numVote == min){
                    loserIndex.push_back(candidateList[i]);
                    candidateList[i].isLoser = true;
                }



            }

            if( max == (ballotNum / candidateCounter) )
                return remainIndex;

        }


        


        if(max >= (ballotNum / 2) ){
            return winnerIndex;
        }

      


        //If no winner for this round
        int ballotNumRow; //which ballot # votes this loser candidate
        int ballotCol;    //which column last read

        for(int i = 0; i < loserIndex.size(); i++){

            for(int j = 0; j < loserIndex[i].ballotChoice.size(); j++){
                ballotNumRow = loserIndex[i].ballotChoice[j].first;
                ballotCol    = loserIndex[i].ballotChoice[j].second;
                loserIndex[i].numVote = 0; //set the loser vote num = 0

                //count for the loser ballot votes, give them to other candidates
                for(int k = ballotCol + 1; k < candidTotalNum; k++){

                       if( ! candidateList[ ballot[ballotNumRow][ballotCol] ].isLoser ){
                            candidateList[ ballot[ballotNumRow][ballotCol] ].numVote++; //Add Vote to the candidate who is not a loser but a next preference on current ballot 
                            candidateList[ ballot[ballotNumRow][ballotCol] ].ballotChoice.first = ballotNumRow;
                            candidateList[ ballot[ballotNumRow][ballotCol] ].ballotChoice.second = k;
                       } 
                }

            }
            
        }

        firstRound = false;
        max = 0;
        min = ballotNum;

    }




    


	return winnerIndex;
}




// -------------
// voting_solve
// -------------

void voting_solve (std::istream& r, std::ostream& w) {
    //*******************TODO*************
    //put case Num here

    int i = 5;

    while (i > 0) {


        
        int ballot[1000][20] = {};
        string candidName[20] = {};
        int candidTotalNum = 0;
        int ballotNum = 0;
        int caseNum = 0;





        bool check = voting_read(r, ballot, candidName, candidTotalNum, ballotNum, caseNum);
        //vector<string> winner = voting_eval();
   		//cout << winner << endl;
        //voting_print(w, p.first, p.second, v);
        i--;
    }
}
