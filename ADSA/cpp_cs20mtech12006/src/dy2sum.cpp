/*
 * Created on Mon Nov 23 2020
 * Author: Bhavishya Sharma
 *
 * Copyright (c) 2020 IIT Hyderabad
 */

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <math.h>

#include "rb_tree.cpp"

class Dy2Sum{
    RBTree<long> tree;

public:
    void insertValue(long value){
        tree.insert(value);
    }
    void deleteValue(long value){
        tree.deleteByValue(value);
    }
    long query(long a, long b){
        RBTree<long> targets;
        RBTreeNode<long>** array = this->tree.getSortedList();
        long count = this->tree.count();
        if(abs(b-a)<= count){
            for(long i=a;i<=b;i++){
                long xIndex=0, yIndex = count-1;
                while(xIndex<yIndex){
                    if(array[xIndex]->data + array[yIndex]->data == i){
                        targets.insertUnique(i);
                        break;
                    }
                    else if(array[xIndex]->data + array[yIndex]->data < i){
                        xIndex++;
                    }
                    else{
                        yIndex--;
                    }
                }
            }
        }
        else{
            for(long i=0;i<count;i++){
                for(long j=i+1;j<count;j++){
                    long sum = array[i]->data + array[j]->data;
                    if(sum >= a && sum <=b){
                        targets.insertUnique(sum);
                    }
                }
            }
        }
        return targets.count();
    }
};

int main(){
    Dy2Sum dySet;
    char ch;
    long value, a, b;
    while((ch = getchar()) != 'E'){
        switch (ch)
        {
        case 'I':
            std::cin>>value;
            dySet.insertValue(value);
            break;
        case 'D':
            std::cin>>value;
            dySet.deleteValue(value);
            break;
        case 'Q':
            std::cin>>a>>b;
            std::cout<<dySet.query(a,b)<<std::endl;
            break;
        default:
            break;
        }
    }
}