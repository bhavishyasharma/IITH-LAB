#include<iostream>
#include<sys/types.h>

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

int main(){
    u_int64_t x = 9875828;
    u_int64_t y = 100000;
    std::cout<<power(x,y)<<std::endl;
}
