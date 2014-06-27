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


int caseNum;
int blank;
bool firstCase;



// ------------
// voting_read
// ------------

vector<string> voting_read (std::istream& r) {


    string line;
    istringstream iss;
  
    int ballot[1000][20] = {};
    string candidName[20] = {};

    vector<string> result;
    
    int candidNum = 0;
    int candidTotalNum = 0;
    int ballotNum = 0;

    bool blank = false;
    bool candidNumCheck = false;




    int c = 0; //index for candidName array

    while (getline(r, line)){
        iss.clear();
        iss.str(line);

        if(firstCase){
            caseNum = atoi(line.c_str());
            firstCase = false;
        }

        else{
        
            //check for blank line

            if(line.empty() ||  (line[0] == '\n') || ( (int)line[0] == 0) || (int)line[0] == 32 ) { 

                    candidNumCheck = true;
                    blank = true;
            }
            else{
                
                if(candidNumCheck){ 
                    //if it is on the candidate number line
                    candidTotalNum = atoi(line.c_str());

                    cout << "candidTotalNum: " << candidTotalNum << endl;

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


                        if(blank){

                            if(r.peek() == '\n' || int(r.peek()) == -1 || int(r.peek()) == 32 ){
                                result = voting_eval(ballot, candidName, candidTotalNum, ballotNum);
                                return result; 
                            }
                        }
                	}
                }// end of checking token array
                
            }

        }// end for first else

    }


    return result; 
}
	

// -------------
// voting_print
// -------------


void voting_print (std::ostream& w, vector<string> result) {

    for(unsigned int i = 0; i < result.size(); i++){

        if(caseNum == 1 && i == result.size() -1)
            w << result[i];
        else
            w << result[i] << endl;
    }

    if(caseNum != 1)
        w << endl;

}


// -------------
// voting_eval
// -------------


vector<string> voting_eval(int ballot[1000][20], string candidName[20], int candidTotalNum, int ballotNum){
	

    vector<string> winnerIndex; //save the name in CandidateList
    vector<int> winnerIndexNum; //store the index
    vector<int> loserIndex; //save the index in CandidateList
    vector<string> remainIndex;
    
    vector<Candidate> candidateList;


    int max = 0;
    int min = ballotNum;

    int temp = 0;
 
    bool firstRound = true;


    //Initialize all candidates
    for(int i = 0; i < candidTotalNum; i++){
        
        struct Candidate c;

        c.name = candidName[i];
        c.numVote = 0;
        c.isLoser = false; 
        c.ballotChoice.clear();

        candidateList.push_back(c);
    }



    //count vote for 1st round(column)
    for(int j = 0; j < ballotNum; j++){

            //cout << "count votes 1st time" << endl;

            //since the candidate # on ballot doesn't start from 0
            temp = ballot[j][0] - 1;
            candidateList[temp].numVote++;

            //update the ballot # and ballot column ptr
            candidateList[temp].ballotChoice.push_back(make_pair(j, 0));

 
    }


    while(true){


        int remainCandidateCounter = 0; //only for not 1st round usage

        for(int i = 0; i < candidTotalNum; i++){


            if(candidateList[i].numVote != 0){
                remainCandidateCounter++;
            }
            
            //trying to find max winner
            if(candidateList[i].numVote > max){

                // if(winnerIndex.empty() == false){
                //     for(unsigned int i = 0; i < winnerIndex.size(); i++){
                //         if(candidateList[i].numVote < min){
                //             loserIndex.clear();
                //             min = candidateList[i].numVote;
                //             loserIndex.push_back(i);
                //         }
                //         else if(candidateList[i].numVote == min){
                //             loserIndex.push_back(i);
                //         }
                //     }
                // }

                winnerIndex.clear();
                max = candidateList[i].numVote;
                winnerIndex.push_back(candidateList[i].name);
                winnerIndexNum.push_back(i);
            } 
            else if(candidateList[i].numVote == max){
                winnerIndex.push_back(candidateList[i].name);
                winnerIndexNum.push_back(i);
            }


            //also check for min, since it might be min

            //trying to find min loser
            // it's possible the candidate is a winner but didn't get any vote at second round, we are trying to find new losers
            if(candidateList[i].numVote < min && candidateList[i].numVote != 0){
                loserIndex.clear();
                min = candidateList[i].numVote;
                loserIndex.push_back(i);
            }
            else if(candidateList[i].numVote == min && candidateList[i].numVote != 0){
                loserIndex.push_back(i);
            }


        }

        if(firstRound){ // only for 1st round since loser's vote will be allocated to others in the other rounds
                //Check for the candidate doesn't get any vote during 1st preference round
            for(int i = 0; i < candidTotalNum; i++){

                if(candidateList[i].numVote == 0){
                    loserIndex.push_back(i);
                    candidateList[i].isLoser = true;
                }
            }
            firstRound = false;
        }

        //check for everone tied
        if((ballotNum % remainCandidateCounter) == 0 && max == (ballotNum / remainCandidateCounter)){
          
            //return anyone who is not loser
            for(int i= 0; i < candidTotalNum; i++){
                if(candidateList[i].isLoser == false){
                    remainIndex.push_back(candidateList[i].name);
                }

            }
            return remainIndex;
        }

        // > 50%
        if(ballotNum % 2 == 0){
            if(max >= (ballotNum / 2) ){
                return winnerIndex;
            }
        }
        
        else{
            if(max >= ((ballotNum / 2) +1) ){
                return winnerIndex;
            }
           
        } 

        //no winner yet
        voting_arrange_loser_vote (candidateList, ballot, loserIndex);
        
            
     



        max = 0;
        min = ballotNum;

    }//end of while(true)

	return winnerIndex; //fake
}


//return vector for testing purpose only
vector<Candidate> voting_arrange_loser_vote (vector<Candidate> &candidateList, int ballot[1000][20], vector<int> loserIndex){

    //If no winner for this round
        int ballotNumRow; //which ballot # votes this loser candidate
        int ballotCol;    //which column last read
        int temp = 0;

        //get every loser candidate in loserIndex
        for(unsigned int i = 0; i < loserIndex.size(); i++){

            candidateList[ loserIndex[i] ].isLoser = true; //loserIndex returns the index of loser candidate in candidateList
            candidateList[ loserIndex[i] ].numVote = 0; //set the loser vote num = 0

            //get each vote for this loser
            for(unsigned int j = 0; j < candidateList[ loserIndex[i] ].ballotChoice.size(); j++){

                ballotNumRow = candidateList[ loserIndex[i] ].ballotChoice[j].first; //the ballot # who votes for this loser (row index)
                ballotCol    = candidateList[ loserIndex[i] ].ballotChoice[j].second; //last reading point for this ballot (col index)
    
                bool addedVote = false;
                int k =  ballotCol + 1;

                //count for the loser ballot votes, give them to other candidates
                
                while(addedVote == false){

                    temp = ballot[ballotNumRow][k] -1; //since candidate # doesn't start from zero index

                       if(candidateList[ temp ].isLoser == false){

                            candidateList[ temp ].numVote++; //Add Vote to the candidate who is not a loser but the next preference on current ballot 
                            
                            candidateList[ temp ].ballotChoice.push_back(make_pair(ballotNumRow, k));
                            addedVote = true;

                       } 

                    k++;
                }

            }
        }


        return candidateList;
}



// -------------
// voting_solve
// -------------

void voting_solve (std::istream& r, std::ostream& w) {
    //*******************TODO*************
    //put case Num here

    caseNum = 1; //temp, it will change after 1st read
    blank = 0;
    firstCase = true;

    cout << "---------------------start-------------------" << endl;

    while (caseNum > 0) {


        vector<string> result = voting_read(r);

        voting_print(w, result);

        caseNum--;
    }
}
