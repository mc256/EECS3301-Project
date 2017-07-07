/*
==================================================
EECS3301 Project 1 Version A

Jun Lin Chen    chen256@my.yorku.ca
Vishal Malik    vishal27@my.yorku.ca
Tong Wu         malan52.82@gmail.com
==================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


/*====================================================*/
/*         Data Type and Variable Declaration         */
/*====================================================*/
#define MAX_LEXEME_LENGTH 100

#define CLASS_OTHERS 0
#define CLASS_LETTER 1
#define CLASS_DIGIT 2
#define CLASS_SYMBOL 3

struct Lexeme{
    int tokenClass;
    char * value;
    size_t length;
    struct Lexeme * next;
};

struct Lexeme * lexemeList;


/*========================================*/
/*        Buffer & Lexical Analysis       */
/*========================================*/

/*
Create New Lexeme String Function
    Allocate memory space for Lexeme. The lexeme will be stored as a string (char[]).
    Maximum size is defined by MAX_LEXEME_LENGTH.
    Return:
        pointer to char[]. the string of lexeme
*/
char * createNewLexemeString(){
    char * temp = malloc(MAX_LEXEME_LENGTH);
    return temp;
}


/*
Create New Lexeme Structure Function
    Allocate memory space for lexeme structure. One lexeme structure contains one lexeme and its character class.
    Return:
        pointer to a empty and new lexeme structure
*/
struct Lexeme * createNewLexeme(){
    struct Lexeme * temp = malloc(sizeof(struct Lexeme));
    temp->length = 0;
    temp->value = createNewLexemeString();
    return temp;
}

/*
Read Source Function
    Read script from standard input. And store it in buffer (lexemeList).
    This is like a light compiling process.
    Argument:
        pointer - allows you to continue writing on the buffer.
*/
void readSource(struct Lexeme * pointer){
    if (pointer == NULL){
        pointer = createNewLexeme();
        lexemeList = pointer;
    }

    bool appendLexeme = false;
    char c;
    while ((c = getchar()) != EOF){
        if (isdigit(c)) {
            if (!appendLexeme){
                pointer->next = createNewLexeme();
                pointer = pointer->next;
                pointer->tokenClass = CLASS_DIGIT;
                appendLexeme = true;
            }            
            if (pointer->length < MAX_LEXEME_LENGTH){ // avoid out of index problem
                pointer->value[pointer->length ++] = c;
                pointer->value[pointer->length] = '\0';
            }
        }else if (isalpha(c)) {
            if (!appendLexeme){
                pointer->next = createNewLexeme();
                pointer = pointer->next;
                pointer->tokenClass = CLASS_LETTER;
                appendLexeme = true;
            }        
            if (pointer->length < MAX_LEXEME_LENGTH){ // avoid out of index problem    
                pointer->value[pointer->length ++] = c;
                pointer->value[pointer->length] = '\0';
            }
        }else if (ispunct(c)){
            pointer->next = createNewLexeme();
            pointer = pointer->next;
            pointer->tokenClass = CLASS_SYMBOL;
            pointer->value[pointer->length ++] = c;
            pointer->value[pointer->length] = '\0';
            appendLexeme = false;
        }else{
            appendLexeme = false;
            continue;
        }
    }

}

/*========================================*/
/*                  MAIN                  */
/*========================================*/

//Main Function
int main(){
    readSource(NULL);
    struct Lexeme * p = lexemeList;
    while ((p = p->next)!=NULL){
        printf("Lexeme is %s\n", p->value);
    }
    return 0;
}
