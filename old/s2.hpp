#include <iostream>
#include <sstream>
#include <string>

// Character classes
#define CLASS_LETTER 1
#define CLASS_DIGIT 2
#define CLASS_SYMBOL 3
#define CLASS_OTHERS 4

// Token codes
#define CODE_EQUAL 1
#define CODE_ADD 2
#define CODE_MINUS 3
#define CODE_MULTIPLICATION 4
#define CODE_SEMICOLON 5

#define CODE_S_PRINT 11
#define CODE_S_LABEL 12
#define CODE_S_GOTO 13
#define CODE_S_IFPOS 14

// A struct you can store token
// And later you will construct it to a link list
struct Token{
    int tokenClass;
    int tokenCode;
    std::stringstream value;  
    struct Token * next;
};

// External Variable
extern struct Token * head;

// Declare function
struct Token * lexicalAnalysis(struct Token * currentToken);