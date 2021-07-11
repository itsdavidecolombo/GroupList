//
//  groups.h
//  
//
//  Created by Davide Colombo on 11/07/21.
//

#ifndef groups_h
#define groups_h

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER 99
#define STRING 999
#define COMMENT 9999
#define OTHER 99999
#define ERROR -99

#define WORDLEN 100

int readword(char *, int);
int binsearch(char **, char *, int);

#endif /* groups_h */
