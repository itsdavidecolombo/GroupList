//
//  groups.c
//  
//
//  Created by Davide Colombo on 11/07/21.
//

#include "groups.h"

#define NKEYS 32
#define MAXWORD 10

/* keyword define by ANSI and ISO C90 */
char *keywords[NKEYS] = {
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",
};

struct group {
    char *pattern;                  /* array of char that contains the pattern */
    char **words;                   /* array of pointers to char that contains the words that matches the pattern */
    int nwords;                     /* pointer to the next free cell for inserting a new word */
    struct group *next;             /* pointer to the next group in the list */
    struct group *prev;             /* pointer to the previous group in the list */
};

int patternlen;
    
char **walloc(void);                /* a function that allocates memory for an array of pointers to strings (pointers to char) */
char *strdup_(char *);              /* a function that duplicates and allocate memory for a single character array */
char *copypattern(char *);          /* a function that allocates memory for an array of character equal to the length of the pattern */

struct group *addgroup(struct group *, char *);
struct group *galloc(void);

void printgroup(struct group *);

int main(int argc, char *argv[]){
        
    /* args check */
    if(argc != 2){
        printf("Usage: ./a.out n\n");
        return 1;
    }else if(isdigit(**(argv+1))){
        patternlen = atoi(*(argv+1));
    }else{
        printf("argv[1] must be a digit\n");
        return 1;
    }
    
    /* Program starts here */
    struct group *root = NULL;
    char word[WORDLEN];
    int type;
    
    while((type = readword(word, WORDLEN)) != EOF){
        if(isalpha(type))
            if(binsearch(keywords, word, NKEYS) == 0)           /* not a C keyword */
                root = addgroup(root, word);
    }
    printgroup(root);
    
    return 0;
}

void printgroup(struct group *g){
    int i;
    if(g != NULL){
        printf("- GROUP PATTERN: \'%s\' -\n", g->pattern);
        for(i = 0; *(g->words+i) != NULL; i++)
            printf("word_%d: %s\n", i, *(g->words+i));
        printgroup(g->next);
    }
}

struct group *addgroup(struct group *g, char *word){
    
    int cond;
    struct group *temp;
    if(g == NULL){
        g = galloc();
        g->nwords = 0;
        g->pattern = copypattern(word);
        g->next = g->prev = NULL;
        g->words = walloc();                        /* dynamically allocate memory for an array of pointers to pointers to char */
        g->words[g->nwords++] = strdup_(word);      /* access to the element pointed by 'words' */
    }else if((cond = strncmp(g->pattern, word, patternlen)) == 0){
        g->words[g->nwords++] = strdup_(word);      /* if the word matches with the pattern, add the word to this group structure */
    }else if(cond < 0){
        g->next = addgroup(g->next, word);
        g->next->prev = g;
    }else{
        if(g->prev == NULL){
            g->prev = addgroup(g->prev, word);
            g->prev->next = g;
            return g->prev;
        }else{
            temp = g->prev;
            g->prev = addgroup(NULL, word);
            g->prev->next = g;
            g->prev->prev = temp;
            temp->next = g->prev;
            return g->prev;
        }
    }
        
    return g;                                       /* the root is the left-most element in the list */
}

char *copypattern(char *word){
    extern int patternlen;
    int i;
    char *p = (char *)malloc(patternlen);
    if(p != NULL)
        for(i = 0; i < patternlen; i++)           /* copy only the first 'patternlen' char into 'p' */
            *(p+i) = *(word+i);
    *(p+patternlen) = '\0';
    return p;
}

/* a function for dynamically allocate space in memory for a new group */

struct group *galloc(void){
    return (struct group *)malloc(sizeof(struct group));
}

/* this function allocate memory for an array of pointers to pointers to char of length equal to MAXWORD */

char **walloc(void){
    char **w;
    w = (char **)malloc(sizeof(char *) * MAXWORD);
    return w;
}

char *strdup_(char *word){
    char *ptr;
    ptr = (char *)malloc(strlen(word)+1);           /* +1 for '\0' */
    if(ptr != NULL)
        strcpy(ptr, word);
    return ptr;
}
