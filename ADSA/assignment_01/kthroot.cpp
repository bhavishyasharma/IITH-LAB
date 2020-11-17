//
// Created by Bhavishya Sharma on 04/10/20.
//

#include <iostream>
#include <sys/types.h>

u_int64_t power(u_int64_t n, u_int64_t k){
    if(k==0)
        return 1;

    // Calculate k/2 power
    u_int64_t pow = power(n, k/2);
    if(k%2==0){
        return pow*pow; //even power
    }
    else{
        return n*pow*pow; // odd power
    }
}

u_int64_t kthroot(u_int64_t n, u_int64_t k){
    if(k==0 || n==0){
        return 0;
    }
    u_int64_t left=0, right=n, mid;
    while(left<right){
        mid = (left+right)/2;
        u_int64_t pow = power(mid, k);
        if(pow==n){
            return mid;
        }
        else if(pow<n){
            left = mid+1;
        }
        else{
            right = mid;
        }
    }
    return left-1;
}

int main(int argc, char **argv) {
    u_int64_t t, n, k;
    std::cin>>t;
    for(u_int64_t i;i<t;i++){
        std::cin>>n>>k;
        std::cout<<kthroot(n,k)<<std::endl;
    }
    return 0;
}