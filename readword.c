//
//  readword.c
//  
//
//  Created by Davide Colombo on 11/07/21.
//

#include "groups.h"

#define BUFSIZE 100

char buf[BUFSIZE];
char *bufp = buf;

int gettype(int);
int getch(void);
void ungetch(int);
int raiseerror(char *);

int readword(char *word, int lim){
    
    char *w = word;
    int c;
    while((c = getch()) == ' ' || c == '\t')
        ;
    
    if(c != EOF)
        *w++ = c;
    if(!isalpha(c)){
        switch(gettype(c)){
            case NUMBER:
            case STRING:
            case COMMENT:
                return c;
            case OTHER:
                *w = '\0';
                return c;
            case ERROR:
                return EOF;
            default:
                break;
        }
    }
    
    for(; --lim > 0 && isalnum(c = getch()); w++)
        *w = c;
    ungetch(c);
    
    *w = '\0';
    return word[0];
}

/* a function that return the type of the character read */

int gettype(int c){
    
    if(isdigit(c)){
        while(isdigit(c = getch()))
            ;
        ungetch(c);
        return NUMBER;
    }else if(c == '\"'){
        while((c = getch()) != '\"' && c != EOF && c != '\n')
            ;
        if(c == EOF || c == '\n')
            return raiseerror("missing closing '\"'\n");
        return STRING;
    }else if(c == '/'){
        if((c = getch()) == '/'){
            while((c = getch()) != '\n')
                ;
            return COMMENT;
        }else if(c == '*'){
            while((c = getch()) != '*' && c != EOF && c != '\n')
                ;
            if(c == '\n' || c == EOF)
                return raiseerror("missing closing '*/' in multiline comment\n");
            if((c = getch()) != '/')
                return raiseerror("missing closing '/' in multiline comment\n");
            return COMMENT;
        }else
            return raiseerror("missing '/' or '*' in comment\n");
    }
    return OTHER;
}

/* a function for raising errors */

int raiseerror(char *msg){
    printf("syntax error: %s\n", msg);
    return ERROR;
}

/* a function for getting char from buffer (if any) or get a char from stdin */

int getch(void){
    return (bufp > buf) ? *--bufp : getchar();
}

/* a function that saves character in the buffer */

void ungetch(int c){
    if(bufp < (buf + BUFSIZE))
        *bufp++ = c;
    else
        printf("ungetch: buffer overflow\n");
}
