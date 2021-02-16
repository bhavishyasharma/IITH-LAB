#include <stdio.h>

#include "lcgrand.h"

float uniform_rand(float low, float high){
    return low + (high-low)*lcgrand(1);
}

int main(){
    float value = 0.0;
    for(int i=0;i<1000;i++){
        value = uniform_rand(4.0, 10.0);
        printf("%f\n", value);
    }
    return 0;
}

