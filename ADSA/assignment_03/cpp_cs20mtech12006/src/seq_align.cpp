/*
 * Created on Fri Dec 04 2020
 * Author: Bhavishya Sharma
 *
 * Copyright (c) 2020 IIT Hyderabad
 */

#include <iostream>
#include <string>

using namespace std;

long** seq_align_cost(char* strM, long m, char* strN, long n, long cG, long cM){
    long **cost = new long*[m+1];
    // Set first column costs
    for(long i=0;i<=m;i++){
        cost[i] = new long[n+1];
        cost[i][0] = i*cG;
    }
    // Set first row costs
    for(long i=0;i<=n;i++){
        cost[0][i] = i*cG;
    }
    for(long i=1;i<=m;i++){
        for(long j=1;j<=n;j++){
            // if characters match
            if(strM[i-1]==strN[j-1]){
                // if penalty after matching is lower
                if(cost[i-1][j-1] < cost[i-1][j]+cG && cost[i-1][j-1] < cost[i][j-1]+cG){
                    cost[i][j] = cost[i-1][j-1];
                }
                // penalty with gap is lower
                else if(cost[i-1][j] < cost[i][j-1]){
                    cost[i][j] = cost[i-1][j] + cG;
                }
                else{
                    cost[i][j] = cost[i][j-1] + cG;
                }
            }
            // characters do not match
            else{
                // penalty after mismatch is lower
                if(cost[i-1][j-1] +cM  < cost[i-1][j]+cG && cost[i-1][j-1] + cM < cost[i][j-1]+cG){
                    cost[i][j] = cost[i-1][j-1] + cM;
                }
                // penalty after gap is lower
                else if(cost[i-1][j] < cost[i][j-1]){
                    cost[i][j] = cost[i-1][j] + cG;
                }
                else{
                    cost[i][j] = cost[i][j-1] + cG;
                }
            }
        }
    }
    return cost;
}

int main(){
    long m, n, cG, cM;
    cin>>m>>n;
    cin>>cG>>cM;
    char* strM = new char[m], *strN = new char[n];
    cin>>strM;
    cin>>strN;
    // find cost matrix
    long ** cost = seq_align_cost(strM, m, strN, n, cG, cM);

    // find alignment using matrix
    string newStrM = "", newStrN = "";
    long i=m, j=n;
    while(i>0 && j>0){
        // if given cost came from matching
        if(cost[i][j] == cost[i-1][j-1] && strM[i-1]==strN[j-1]){
            newStrM=strM[--i]+newStrM;
            newStrN=strN[--j]+newStrN;
        }
        // if given cost came from left
        else if(cost[i][j] == cost[i][j-1] + cG){
            newStrM="_"+newStrM;
            newStrN=strN[--j]+newStrN;
        }
        // if given cost came from top
        else if(cost[i][j] == cost[i-1][j] + cG){
            newStrM=strM[--i]+newStrM;
            newStrN="_"+newStrN;
        } 
        // if given cost came from diagonal/mismatch
        else{
            newStrM=strM[--i]+newStrM;
            newStrN=strN[--j]+newStrN;
        }
    }
    // copy residual characters in strM with gap in N
    while(i>0){
        newStrM=strM[--i]+newStrM;
        newStrN="_"+newStrN;
    }
    // copy residual characters in strN with gap in M
    while(j>0){
        newStrM="_"+newStrM;
        newStrN=strN[--j]+newStrN;
    }
    cout<<cost[m][n]<<endl;
    cout<<newStrM<<endl;
    cout<<newStrN<<endl;
    return 0;
}