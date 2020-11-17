//
// Created by Bhavishya Sharma on 04/10/20.
//

#include <iostream>
#include <sys/types.h>

u_int64_t mergeAndCountInversion(int64_t  arr[], int64_t  left, int64_t  right){
    int64_t  mid=(left+right)/2, counter=0;
    int64_t  sorted[right-left+1];
    int64_t  i=left, j=mid+1, k=0;
    while(i<=mid && j<=right){
        if(arr[i]<=arr[j]){
            sorted[k++] = arr[i++];
        }
        else{
            sorted[k++] = arr[j++];
            counter+=(mid-i+1);
        }
    }
    while(i<=mid){
        sorted[k++] = arr[i++];
    }
    while(j<=right){
        sorted[k++] = arr[j++];
    }
    for(i=left, k=0;i<=right;i++, k++){
        arr[i] = sorted[k];
    }
    return counter;
}

int64_t  countInversion(int64_t  arr[], int64_t  left, int64_t  right){
    if(left >= right)
        return 0;
    int64_t  mid = (left+right)/2;
    int64_t  counter = 0;
    counter += countInversion(arr, left, mid);
    counter += countInversion(arr, mid+1, right);
    counter += mergeAndCountInversion(arr, left, right);
    return  counter;
}

int main(int argc, char **argv) {
    int64_t n;
    std::cin>>n;
    int64_t arr[n], sorted[n];
    for(int64_t  i=0;i<n;i++){
        std::cin>>arr[i];
    }
    std::cout<<countInversion(arr, 0, n-1)<<std::endl;
    return 0;
}