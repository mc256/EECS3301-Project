#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


/*********************************
    Data Type and Variable
 **********************************/
#define CLASS_LETTER 1
#define CLASS_DIGIT 2
#define CLASS_SYMBOL 3
#define CLASS_OTHERS 4
#define CLASS_EOF 5

#define CODE_EQUAL 1
#define CODE_ADD 2
#define CODE_MINUS 3
#define CODE_MULTIPLICATION 4
#define CODE_SEMICOLON 5

#define CODE_S_PRINT 11
#define CODE_S_LABEL 12
#define CODE_S_GOTO 13
#define CODE_S_IFPOS 14

struct Buffer {
    char * data;
    size_t length;  //Include '\0'
};

struct Buffer * buffer;

struct Label {
    char * labelName; // The first character of the label name
    size_t length; // Length of the label name
    char * nextCommand;
    struct Label * next;
};

struct Label * labelList;

struct Variable {
    char * variableName;
    size_t length;
    int value;
    struct Variable * next;
};

struct Variable * variableList;

/*********************************
    Utility
 **********************************/

void printError(char * error){
    printf("Error:\n\t%s\n", error);
    exit(1);
}

/*********************************
    Buffer Operation
 **********************************/
/*
 Initialize Buffer Function
    This function initialize buffer for the entire script.
 */
bool initializeBuffer(){
    buffer = malloc(sizeof(struct Buffer));
    if (buffer == NULL){
        return false; // Not enough memory
    }
    // Initialize
    buffer->length = 1;
    buffer->data = malloc(1);
    if (buffer->data == NULL) {
        return false; // Not enough memory
    }
    buffer->data[0] = '\0';
    
    return true;
}

/*
 Append Buffer Function
    This function initialize buffer for the entire script.
 */
bool appendBuffer(char c){
    // Increase the length of buffer
    char * temp = realloc(buffer->data, buffer->length + 1);
    if (temp == NULL){
        return false; // Not engough memory
    }
    
    //Append character
    buffer->data = temp;
    buffer->data[buffer->length - 1] = c;
    buffer->data[buffer->length] = '\0';
    buffer->length ++;
    
    return true;
}

void readSource(){
    if (!initializeBuffer()){
        printError("Buffer Initialization Failed.");
    }
    char c;
    while ((c = getchar()) != EOF){
        if (!appendBuffer(c)) {
            printError("Not Enough Memory.");
        }
    }
}

/*********************************
    Lexical Analysis
 **********************************/
/*
 similar to lex() in the textbook
 will set
    the char * pointer (in char * lexeme)
    the length of the lexeme (size_t lexeme_length)
    the token type (int token)
 Argument
    char * pointer - starting point
 Return
    char * - pointer after the lexeme; NULL if end of buffer
 */
char * getLexeme(char * pointer){
    bool appendLexeme = false;
    int charClass;
    for(;;){
        char c = * pointer;
        if (c == '\0'){
            if (!appendLexeme){
                lexeme = pointer;
                lexeme_length = 0;
            }
            return NULL;
        }else if (isalpha(c)){
            if (appendLexeme) {
                lexeme_length ++;
            }else{
                charClass = CLASS_LETTER;
                lexeme = pointer;
                lexeme_length = 1;
                appendLexeme = true;
            }
        }else if (isdigit(c)){
            if (appendLexeme) {
                lexeme_length ++;
            }else{
                charClass = CLASS_DIGIT;
                lexeme = pointer;
                lexeme_length = 1;
                appendLexeme = true;
            }
        }else if (ispunct(c)){
            if (appendLexeme) {
                return pointer;
            }else{
                charClass = CLASS_SYMBOL;
                lexeme = pointer;
                lexeme_length = 1;
                return ++ pointer;
            }
        }else{
            if (appendLexeme){
                return ++ pointer;
            }
        }
        
        pointer ++;
    }
    
}

bool checkLexeme(char * keyword){
    return strncmp(keyword, lexeme, lexeme_length) == 0;
}


/*********************************
    Label Analysis
 **********************************/
bool initializeLabelList(){
    labelList = malloc(sizeof(struct Label));
    if (labelList == NULL){
        return false; // Not enough memory
    }
    return true;
}

void parseLabels(char * startingPoint){
    char * readingPointer = startingPoint;
    bool checkLabel = true;
    while ((readingPointer = getLexeme(readingPointer)) != NULL){
        if (checkLabel && checkLexeme("label")){
            
        }else if (checkLexeme(";")){
            checkLabel = true;
        }else {
            checkLabel = false;
        }        
    }
}

char * gotoLabel(char * label){
    return NULL;
}

bool initializeVariableList(){
    variableList = malloc(sizeof(struct Variable));
    if (variableList == NULL){
        return false; // Not enough memory
    }
    return true;
}



void testParse(){
    char * readingPointer = buffer->data;
    while ((readingPointer = getLexeme(readingPointer)) != NULL){
        char * printTemp = malloc(lexeme_length + 1);
        strncpy(printTemp, lexeme, lexeme_length);
        printTemp[lexeme_length] = '\0';
        printf("%s<----\n", printTemp);
        if (compareLexeme("print")) {
            printf("==============This is print.\n");
        }
        free(printTemp);
    }
}



int main(){
    readSource();
    testParse();
    
    if (!(initializeLabelList() && initializeVariableList())) {
        printError("Not Enough Memory");
    }
    

    return 0;
}