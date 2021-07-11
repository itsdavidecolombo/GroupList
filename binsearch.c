//
//  binsearch.c
//  
//
//  Created by Davide Colombo on 11/07/21.
//

#include "groups.h"

int binsearch(char *v[], char *word, int n){
    int cond;
    int low  = 0;
    int high = n-1;
    int mid;
    while(low <= high){
        mid = (low + high) / 2;
        if((cond = strcmp(word, v[mid])) < 0)
            high = mid - 1;
        else if(cond > 0)
            low = mid + 1;
        else{
            return mid;
        }
    }
    return 0;
}
