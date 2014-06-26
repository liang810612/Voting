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






vector<string> result = voting_eval(ballot, candidName, candidTotalNum, ballotNum);

cout << "_______________________result--------------------------------" << endl;

 for(int i = 0; i < result.size(); i++){


    cout << result[i] << endl;

 }

 cout << "-------------------------result end----------------" << endl;

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


vector<string> voting_eval(int ballot[1000][20], string candidName[20], int candidTotalNum, int ballotNum){
	

    vector<string> winnerIndex; //save the name in CandidateList
    vector<int> loserIndex; //save the index in CandidateList
    vector<string> remainIndex; //  for the candidate who is not a loser

    vector<Candidate> candidateList;


    int max = 0;
    int min = ballotNum;

    int temp = 0;
 
    bool firstRound = true;


    //struct Candidate candidateList[candidTotalNum];

   

    //Initialize all candidates
    for(int i = 0; i < candidTotalNum; i++){
        
        struct Candidate c;

        c.name = candidName[i];
        c.numVote = 0;
        c.isLoser = false; 
        c.ballotChoice.clear();

        candidateList.push_back(c);
    }


    cout << "candidateList size at beginning: " << candidateList.size() << endl;


    //count vote for 1st round(column)
    for(int j = 0; j < ballotNum; j++){


            //since the candidate # on ballot doesn't start from 0
            temp = ballot[j][0] - 1;

            candidateList[temp].numVote++;

            //update the ballot # and ballot column ptr
            candidateList[temp].ballotChoice.push_back(make_pair(j, 0));


            //trying to find max winner
            //if this candidate has not been saved into winnerIndex
            if(candidateList[temp].numVote > max){
                winnerIndex.clear();
                max = candidateList[temp].numVote;
                winnerIndex.push_back(candidateList[temp].name); // save the index indicates the candidate in candidateList
            }
            else if(candidateList[temp].numVote == max){
                winnerIndex.push_back(candidateList[temp].name);
            }

            //trying to find min loser
            else if(candidateList[temp].numVote < min){
                loserIndex.clear();
                min = candidateList[temp].numVote;
                loserIndex.push_back(temp);
                //candidateList[temp].isLoser = true;
            }
            else if(candidateList[temp].numVote == min){
                loserIndex.push_back(temp);
                //candidateList[temp].isLoser = true;
            }

 
    }

    //Check for the candidate doesn't get any vote during 1st preference round
    for(int i = 0; i < candidTotalNum; i++){

        if(candidateList[i].numVote == 0){
            loserIndex.push_back(i);
            candidateList[i].isLoser = true;
        }
    }

    //During 1st round, if everyone gets same amount of vote number
    if(max == min){
        for(int i= 0; i < candidTotalNum; i++){
            if(candidateList[i].isLoser == false)
                remainIndex.push_back(candidateList[i].name);

        }
        return remainIndex;
    }


    while(true){


        cout << "while true" << endl;


        int candidateCounter = 0; //only for not 1st round usage

        
        if(firstRound == false){
            //get max and min

            cout << "******Not 1st round" << endl;

            for(int i = 0; i < candidTotalNum; i++){

                

                if(candidateList[i].numVote != 0){
                    candidateCounter++;
                }
                
                //trying to find max winner
                if(candidateList[i].numVote > max){

                    cout << "max: " << candidateList[i].name << " " << candidateList[i].numVote << endl;

                    winnerIndex.clear();
                    max = candidateList[i].numVote;
                    winnerIndex.push_back(candidateList[i].name);
                } 
                else if(candidateList[i].numVote == max){
                    winnerIndex.push_back(candidateList[i].name);
                }


                //trying to find min loser
                // it's possible the candidate is a winner but didn't get any vote at second round, we are trying to find new losers
                else if(candidateList[i].numVote < min && candidateList[i].numVote != 0){
                    loserIndex.clear();
                    min = candidateList[i].numVote;
                    loserIndex.push_back(i);
                }
                else if(candidateList[i].numVote == min){
                    loserIndex.push_back(i);
                }



            }


            //check for everone tied
            if((ballotNum % candidateCounter) == 0 && max == (ballotNum / candidateCounter)){
              
                //return anyone who is not loser
                for(int i= 0; i < candidTotalNum; i++){
                    if(candidateList[i].isLoser == false){
                        remainIndex.push_back(candidateList[i].name);
                    }

                }
                return remainIndex;
             
            }


            


        }


        

        // > 50%
        cout << "ballotNum: " << ballotNum << endl;
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
        
        

      


        //If no winner for this round
        int ballotNumRow; //which ballot # votes this loser candidate
        int ballotCol;    //which column last read

        cout << "loserIndex size: " << loserIndex.size() << " " << loserIndex[0] << endl;

        for(int i = 0; i < loserIndex.size(); i++){

            candidateList[ loserIndex[i] ].isLoser = true; //loserIndex returns the index of loser candidate in candidateList
            candidateList[ loserIndex[i] ].numVote = 0; //set the loser vote num = 0

            cout << candidateList[ loserIndex[i] ].name << endl;


            for(int j = 0; j < candidateList[ loserIndex[i] ].ballotChoice.size(); j++){

                ballotNumRow = candidateList[loserIndex[i]].ballotChoice[j].first;
                ballotCol    = candidateList[loserIndex[i]].ballotChoice[j].second;
    
                bool addedVote = false;
                int k =  ballotCol + 1;
                //count for the loser ballot votes, give them to other candidates
                
                while(addedVote == false){
                //for(int k = ballotCol + 1; k < candidTotalNum; k++){

                    temp = ballot[ballotNumRow][k] -1;

                       if(candidateList[ temp ].isLoser == false){

                            cout << "candidateList[ temp ].name: " << candidateList[ temp ].name << endl;
                            candidateList[ temp ].numVote++; //Add Vote to the candidate who is not a loser but a next preference on current ballot 
                            
                            candidateList[ temp ].ballotChoice.push_back(make_pair(ballotNumRow, k));
                            addedVote = true;

                       } 

                    k++;
                }

            }
            
        }

        // cout << "1st round remainIndex" << endl;

        // for(int i= 0; i < candidTotalNum; i++){
        //     if(candidateList[i].isLoser == false)
        //         cout << candidateList[i].name << endl;

        // }
        // cout << "####################################" << endl;

        firstRound = false;
        max = 0;
        min = ballotNum;


        cout << "while true end" << endl;

    }




    


	return winnerIndex;
}




// -------------
// voting_solve
// -------------

void voting_solve (std::istream& r, std::ostream& w) {
    //*******************TODO*************
    //put case Num here

    int i = 2;

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
