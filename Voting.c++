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

int defaultCaseNum;
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
    vector<string> candidName {};

    vector<string> result {};
    
    int candidNum = 0;
    int candidTotalNum = 0;
    int ballotNum = 0;
    int candidNumCount = 0;

    bool blank = false;
    bool candidNumCheck = false;
    bool checkBallot = false;
    bool checkCandidate = false;




    while (getline(r, line)){
        iss.clear();
        iss.str(line);

        if(firstCase){
            defaultCaseNum = atoi(line.c_str());
            caseNum = defaultCaseNum;
            firstCase = false;

            if(caseNum == 0)
                return result;
        }

        else{

      
            //check for blank line

            if(line.empty() ||  (line[0] == '\n') || ( (int)line[0] == 0) ) { 
                    

                    candidNumCheck = true;
                    blank = true;
            }
            else{
                
                if(candidNumCheck){ 
                    //if it is on the candidate number line
                    candidTotalNum = atoi(line.c_str());
                    candidNumCount = candidTotalNum;

                    if(candidTotalNum == 0)
                        return result;

                    candidNumCheck = false; 

                }
                else{


                // check and put tokens into specific array
                    if(candidNumCount > 0){
                        checkCandidate = true;

                        //It's checking candidate's name
                        candidName.push_back(line);
                        candidNumCount--;

                        if(r.peek() == '\n' || int(r.peek()) == -1){

                            if(checkBallot == false && checkCandidate == true){
                                //no ballot
                                result = voting_eval(ballot, candidName, candidTotalNum, ballotNum);
                                return result; 
                            }
                        }
                    }
                
                    else{  

                        //It's checking ballot
                        checkBallot = true;

                        string token;
                        stringstream s(line);
                        while(getline(s, token, ' ')){

                            ballot[ballotNum][candidNum] = atoi(token.c_str());
                            candidNum++;
                        }

                        ballotNum++;
                        candidNum  = 0;


                        if(blank){

                            if(r.peek() == '\n' || int(r.peek()) == -1){

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


            w << result[i] << endl;
    }

    if(caseNum != 1)
        w << endl;

}


// -------------
// voting_eval
// -------------


vector<string> voting_eval(int ballot[1000][20], vector<string> candidName, int candidTotalNum, int ballotNum){
    

    vector<string> winnerIndex {}; //save the name in CandidateList
    vector<int> loserIndex {}; //save the index in CandidateList
    vector<string> remainIndex {};
    
    vector<Candidate> candidateList {};


    int max = 0;
    int min = ballotNum;

    int temp = 0;
 
    bool firstRound = true;


    if(ballotNum == 0){
        for(int i = 0; i < candidTotalNum; i++)
            winnerIndex.push_back(candidName[i]);

        return winnerIndex;
    }

    //Initialize all candidates
    for(int i = 0; i < candidTotalNum; i++){
        
        struct Candidate c;
        vector<pair<int, int>> p {};

        c.name = candidName[i];
        c.numVote = 0;
        c.isLoser = false; 
        c.ballotChoice = p;

        candidateList.push_back(c);
    }
    //count vote for 1st round(column)
    for(int j = 0; j < ballotNum; j++){

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

                winnerIndex.clear();
                max = candidateList[i].numVote;
                winnerIndex.push_back(candidateList[i].name);

            } 
            else if(candidateList[i].numVote == max){
                winnerIndex.push_back(candidateList[i].name);

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
            if(max >= ((ballotNum / 2) + 1) ){
                return winnerIndex;
            }
           
        } 

        //no winner yet
        voting_arrange_loser_vote (candidateList, ballot, loserIndex, candidTotalNum);


        max = 0;
        min = ballotNum;

    }//end of while(true)

    return winnerIndex; //fake
}


//return vector for testing purpose only
vector<Candidate> voting_arrange_loser_vote (vector<Candidate> &candidateList, int ballot[1000][20], vector<int> loserIndex, int candidTotalNum){

    //If no winner for this round
        int ballotNumRow = 0; //which ballot # votes this loser candidate
        int ballotCol = 0;    //which column last read
        int temp = 0;
        bool addedVote = false;
        int k = 0;

          

        //get every loser candidate in loserIndex
        for(int i = 0; i < loserIndex.size(); i++){


            candidateList.at(loserIndex.at(i)).isLoser = true; //loserIndex returns the index of loser candidate in candidateList
            candidateList.at(loserIndex.at(i)).numVote = 0; //set the loser vote num = 0


            if(candidateList.at(loserIndex.at(i)).ballotChoice.size() > 0){

                //get each vote for this loser
                for(int j = 0; j < candidateList.at(loserIndex.at(i)).ballotChoice.size(); j++){

                    pair <int,int> tempB = candidateList.at(loserIndex.at(i)).ballotChoice.at(j);

                    ballotNumRow = tempB.first; //the ballot # who votes for this loser (row index)
                    ballotCol    = tempB.second; //last reading point for this ballot (col index)
        
                    addedVote = false;
                    if(ballotCol < 19)
                        k =  ballotCol + 1;
                    else
                        k = ballotCol;

                    //count for the loser ballot votes, give them to other candidates
                    while(addedVote == false && k < candidTotalNum){

                        temp = ballot[ballotNumRow][k] -1; //since candidate # doesn't start from zero index

                           if(temp < candidateList.size()){
                                if(candidateList.at(temp).isLoser == false){
                                    candidateList.at(temp).numVote++; //Add Vote to the candidate who is not a loser but the next preference on current ballot 
                                    candidateList.at(temp).ballotChoice.push_back(make_pair(ballotNumRow, k));
                                    addedVote = true;
                                }
                           }

                        k++;
                    }

                } //end of for
            }
            
        }


        return candidateList;
}



// -------------
// voting_solve
// -------------

void voting_solve (std::istream& r, std::ostream& w) {

    caseNum = 1; //temp, it will change after 1st read
    blank = 0;
    firstCase = true;
    
    

    while (caseNum > 0) {

        vector<string> result = voting_read(r);
        
        voting_print(w, result);

        caseNum--;
    }

    if(defaultCaseNum == 0)
        w << endl;
}
