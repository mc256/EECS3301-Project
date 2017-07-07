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

struct Label {
    struct Lexeme * labelName;
    struct Lexeme * nextCommand;
    struct Label * next;
};

struct Label * labelList;

struct Variable {
    char * variableName;
    long variableValue;
    struct Variable * next;
};

struct Variable * variableList;

// RDP functions
struct Lexeme * symbolFactor(struct Lexeme * pointer, long * writeBack);
struct Lexeme * symbolTerm(struct Lexeme * pointer, long * writeBack);
struct Lexeme * symbolExpr(struct Lexeme * pointer, long * writeBack);
struct Lexeme * symbolStatementPrint(struct Lexeme * pointer);
struct Lexeme * symbolStatementIfPositive(struct Lexeme * pointer);
struct Lexeme * symbolStatementAssign(struct Lexeme * pointer);
struct Lexeme * symbolStatementLabel(struct Lexeme * pointer);
struct Lexeme * symbolStatementComment(struct Lexeme * pointer);
struct Lexeme * symbolStatement(struct Lexeme * pointer);


/*========================================*/
/*             Utility                    */
/*========================================*/

/*
Print Error Function
    Print out error message (including syntax error messages) and exit the program.
    Argument:
        error - error message
*/
void printError(char * error, char * symbol, char * tokenValue){
    printf("Error:\n");
    if (symbol != NULL){
        printf("\tIn %s lexeme \"%s\".\n",symbol, tokenValue == NULL ? "NULL" : tokenValue);
    }
    printf("\t%s\n", error);
    exit(1);
}

/*
Initialize Function
    Initialize memory for storing labels and variables.
*/
void initialize(){
    labelList = malloc(sizeof(struct Label));
    variableList = malloc(sizeof(struct Variable));
    if (labelList == NULL || variableList == NULL){
        printError("Not Enough Memory", NULL, NULL);
    }
    labelList->next = NULL;
    variableList->next = NULL;
}


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
    if (temp == NULL){
        printError("Not Enough Memory", NULL, NULL);
    }
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
    if (temp == NULL){
        printError("Not Enough Memory", NULL, NULL);
    }
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
/*               Label List               */
/*========================================*/

/*
Create New Label Function
    Allocates memory for label structure.
    Return:
        Label structure
*/
struct Label * createNewLabel(){
    struct Label * temp = malloc(sizeof(struct Label *));
    if (temp == NULL){
        printError("Not Enough Memory", NULL, NULL);
    }
    temp->next = NULL;
    return temp;
}


/*
Add Label Function
    Add label to list. Over write if it exists.
    Argument:
        labelName - the lexeme structure which stores the label name
        nextComment - the lexeme structure which stores the semicolon for the label statement
*/
void addLabel(struct Lexeme * labelName, struct Lexeme * nextComment){
    //second declaration for the same label name will over write the previous one
    struct Label * p = labelList;
    while (p->next != NULL){
        p = p->next;
        if (strcmp(labelName->value, p->labelName->value) == 0){
            p->nextCommand = nextComment;
            return;
        }
    }
    p->next = createNewLabel();
    p = p->next;
    
    p->labelName = labelName;
    p->nextCommand = nextComment;
}

/*
Go To Label Function
    return the pointer to the semicolon of the label statement. Therefore the RDP can execute next command.
    Argument:
        name - of the label
    Return:
        pointer to the semicolon.
*/
struct Lexeme * gotoLabel(char * name){
    struct Label * p = labelList;
    while ((p = p->next) != NULL){
        if (strcmp(name, p->labelName->value) == 0){
            return p->nextCommand;
        }
    }
    printError("Runtime: Lable Not Found",  "label name searching process, for", name);
    return NULL;
}


/*========================================*/
/*            Variable List               */
/*========================================*/

/*
Create New Variable Function
    Allocate memory for Variable structure.
    Return:
        Variable structure.
*/
struct Variable * createNewVariable(){
    struct Variable * temp = malloc(sizeof(struct Variable));
    if (temp == NULL){
        printError("Runtime: Not Enough Memory", NULL, NULL);
    }
    temp->next = NULL;
    return temp;
}

/*
Set Variable Function
    Set the value of a variable. If the variable exists, change its value.
    Argument:
        name - variable name
        value - value of the variable
*/
void setVariable(char * name, long value){
    struct Variable * p = variableList;
    while (p->next != NULL){
        p = p->next;
        if (strcmp(name, p->variableName) == 0){
            p->variableValue = value;
            return;
        }
    }
    p->next = createNewVariable();
    p = p->next;

    p->variableName = name;
    p->variableValue = value;
}

/*
Get Variable Function
    Get the value of a variable.
    Argument:
        name - variable name
    Return:
        value of the variable
*/
long getVariable(char * name){
    struct Variable * p = variableList;
    while ((p = p->next) != NULL){
        if (strcmp(name, p->variableName) == 0)   {
            return p->variableValue;
        }
    }
    printError("Variable Not Defined", "variable searching process, for", name);
    return 0;
}

/*========================================*/
/*            lexicaly Operation          */
/*========================================*/

/*
Check Lexeme Function
    Confirm if the lexeme is what we want.
    Argument:
        lexeme - lexeme structure
        compareTo - lexeme
    Return:
        True if same
*/
bool checkLexeme(struct Lexeme * lexeme, char * compareTo){
    if (lexeme == NULL || lexeme->value == NULL || compareTo == NULL){
        return false;
    }
    return strcmp(lexeme->value, compareTo) == 0;
}

/*
Get Lexeme Function
    Return:
        lexeme itself as string (char[])
*/
char * getLexeme(struct Lexeme * lexeme){
    return lexeme->value;
}




/*========================================*/
/*  Recursive-Descent Parsing Functions   */
/*========================================*/
/*
There is a double semicolon issue in Prof's PDF. So after discussing with Prof we are following these EBNF

<letter> -> A | B | … | Z | a | b | … | z 
<digit> -> 0 | 1 | 2 | 3 | 4 | 5| 6 | 7 | 8 | 9
<int_constant> -> <digit> { <digit> }
<id> -> <letter> { <letter> | <digit> }

<factor> -> id | int_constant | (<expr>)
<term> -> <factor> { * <factor>}
<expr> -> <term> { ( + | - ) <term> } 

<s_print> -> print <expr>
<s_goto> -> goto <id>
<s_ifpos> -> ifpos <expr> goto <id>
<s_assign> -> <id> = <expr>
<s_label> -> label <id>

<statement> -> <s_print> |<s_assign> |<s_label> |<s_goto> |<s_ifpos>
<program> -> <statement> { {;} <statement> } [;]

We also add a comment feature.
<s_comment> -> COMMENT: <anything except semicolon>
<statement> -> <s_print> |<s_assign> |<s_label> |<s_goto> |<s_ifpos> |<s_comment>

In some situation, there are too many options that can be chosen. In each recursive-descent function, there are two parts.

1.  "SOFT" means the RDP has NOT YET confirmed the statement exists in the functions' statements.
    If it cannot recognize the statement, it will return the pointer back to upper level without moving the pointer.

2.  "HARD" means the RDP has confirmed that the statement exists in the functions' statement.It cannot be anything other than this option.
    If it didn't recognize the statement, it will produce a syntax error.

Although the professor said we can have all the keywords reserved, however in this implementation,
all the keywords can be <id>. That means you can have a variable name same as any keyword.
*/

struct Lexeme * symbolFactor(struct Lexeme * pointer, long * writeBack){
    // <factor> -> id | int_constant | (<expr>)
    // HARD
    long value = 0;
    if (checkLexeme(pointer, "(")){
        long cb = 0;
        pointer = symbolExpr(pointer->next, &cb);
        value = value + cb;
        if (!checkLexeme(pointer, ")")){
            printError("Syntax: Expecting Right Parentheses for '(<expr>).'", "<factor>", pointer->value);
        }
        * writeBack = value;
    }else if (pointer->tokenClass == CLASS_LETTER){
        value = getVariable(pointer->value);
    }else if (pointer->tokenClass == CLASS_DIGIT) {
        value = atol(pointer->value);
    }else{
            printError("Syntax: CANNOT Find a valid 'id | int_constant | (<expr>)'", "<factor>", pointer->value);
    }
    *writeBack = value;
    return pointer->next;
}

struct Lexeme * symbolTerm(struct Lexeme * pointer, long * writeBack){
    // <term> -> <factor> { * <factor>}
    // HARD
    long value = 0;
    pointer = symbolFactor(pointer, &value);
    for (;;){
        if (checkLexeme(pointer, "*")) {
            long cb = 0;
            pointer = symbolTerm(pointer->next, &cb);
            value = value * cb;
        }else {
            break;
        }
    }
    *writeBack = value;
    return pointer;
}

struct Lexeme * symbolExpr(struct Lexeme * pointer, long * writeBack){
    // <expr> -> <term> { ( + | - ) <term> } 
    // HARD
    long value = 0;
    pointer = symbolTerm(pointer, &value);
    for (;;){
        if (checkLexeme(pointer, "+")){
            long cb = 0;
            pointer = symbolTerm(pointer->next, &cb);
            value = value + cb;
        }else if (checkLexeme(pointer, "-")){
            long cb = 0;
            pointer = symbolTerm(pointer->next, &cb);
            value = value - cb;
        }else {
            break;
        }
    }
    *writeBack = value;
    return pointer;
}

struct Lexeme * symbolStatementPrint(struct Lexeme * pointer){
    // <s_print> -> print <expr>
    // SOFT
    if (!checkLexeme(pointer, "print")){
        return pointer;    
    }

    if (pointer->next == NULL || (pointer->next->tokenClass != CLASS_DIGIT && pointer->next->tokenClass != CLASS_LETTER && !checkLexeme(pointer->next, "("))) {
        return pointer;
    }

    // HARD
    long value = 0;
    pointer = symbolExpr(pointer->next, &value);
    printf("%ld\n", value);

    return pointer;    
}

struct Lexeme * symbolStatementGoTo(struct Lexeme * pointer){
    // <s_goto> -> goto <id>
    // SOFT
    if (!checkLexeme(pointer, "goto")){
        return pointer;
    }

    if (pointer->next == NULL || pointer->next->tokenClass != CLASS_LETTER) {
        return pointer;
    }

    // HARD
    // We need to check the semicolon here rather than in computeParse() function
    // Because the reading pointer will jump away after return.
    if (pointer->next->next != NULL && !checkLexeme(pointer->next->next, ";")){
        printError("Syntax: '<s_goto> -> goto <id> ;' Expecting ';'.", "<s_goto>", pointer->value);
    }

    return gotoLabel(pointer->next->value);
}


struct Lexeme * symbolStatementIfPositive(struct Lexeme * pointer){
    // <s_ifpos> -> ifpos <expr> goto <id>
    // SOFT
    if (!checkLexeme(pointer, "ifpos")){
        return pointer;
    }

    // FIRST(<expr>) = {a-zA-Z0-9\(}
    if (pointer->next == NULL || checkLexeme(pointer->next, "=")) {
        return pointer;
    }

    // HARD
    long value = 0;
    pointer = symbolExpr(pointer->next, &value);

    if (!checkLexeme(pointer, "goto")){
        printError("Syntax: '<s_ifpos> -> ifpos <expr> goto <id> ;' Expecting 'goto'.", "<s_goto>", pointer->value);
    }

    if (pointer->next == NULL || pointer->next->tokenClass != CLASS_LETTER) {
        printError("Syntax: '<s_ifpos> -> ifpos <expr> goto <id> ;' Expecting '<id>'.", "<s_goto>", pointer->value);
    }

    // We need to check the semicolon here rather than in computeParse() function
    // Because the reading pointer will jump away after return.
    if (pointer->next->next != NULL && !checkLexeme(pointer->next->next, ";")){
        printError("Syntax: '<s_ifpos> -> ifpos <expr> goto <id> ;' Expecting ';'.", "<s_goto>", pointer->value);
    }   
    if (value > 0){
        return gotoLabel(pointer->next->value);
    }
    return pointer->next->next;
}

struct Lexeme * symbolStatementAssign(struct Lexeme * pointer){
    // <s_assign> -> <id> = <expr>
    // SOFT
    if (pointer->tokenClass != CLASS_LETTER){
        return pointer;
    }

    char * name = pointer->value;

    if (pointer->next == NULL || !checkLexeme(pointer->next, "=")){
        return pointer;
    }

    // HARD
    long value = 0;
    pointer = symbolExpr(pointer->next->next, &value);

    setVariable(name, value);

    return pointer;
}

struct Lexeme * symbolStatementLabel(struct Lexeme * pointer){
    // <s_label> -> label <id>
    // We don't do anything here. Because label parsing is in a separate function.
    // SOFT
    if (!checkLexeme(pointer, "label")){
        return pointer;
    }
    if (pointer->next == NULL || pointer->next->tokenClass != CLASS_LETTER){
        return pointer;
    }

    // HARD
    return pointer->next->next;
}

struct Lexeme * symbolStatementComment(struct Lexeme * pointer){
    // <s_comment> -> COMMENT: <anything except semicolon>
    // SOFT
    if (!checkLexeme(pointer, "COMMENT")){
        return pointer;
    }
    if (pointer->next == NULL || !checkLexeme(pointer->next, ":")){
        return pointer;
    }

    // HARD
    while ((pointer = pointer->next) != NULL){
        if (checkLexeme(pointer, ";")){
            return pointer;
        }
    }
    return pointer;
}

struct Lexeme * symbolStatement(struct Lexeme * pointer){
    // <statement> -> <s_print> |<s_assign> |<s_label> |<s_goto> |<s_ifpos> |<s_comment>
    
    struct Lexeme * onHold = pointer;
    if ((pointer = symbolStatementPrint(pointer)) != onHold){
        return pointer;
    }
    if ((pointer = symbolStatementGoTo(pointer)) != onHold){
        return pointer;
    }
    if ((pointer = symbolStatementIfPositive(pointer)) != onHold){
        return pointer;
    }
    if ((pointer = symbolStatementAssign(pointer)) != onHold){
        return pointer;
    }
    if ((pointer = symbolStatementLabel(pointer)) != onHold){
        return pointer;
    }
    if ((pointer = symbolStatementComment(pointer)) != onHold){
        return pointer;
    }
    // HARD
    
    return pointer;
}

/*========================================*/
/*            Parsing Process             */
/*========================================*/

/*
Label Parse Function
    Find all the labels and save them in the label list.
    Argument:
        pointer - the starting point to find
*/
void labelParse(struct Lexeme * pointer){
    bool checkLabelToken = true;
    while ((pointer = pointer->next) != NULL){
        if (checkLexeme(pointer, "label") && checkLabelToken){
            // Check the next two Tokens
            // <id> [;]
            if (pointer->next == NULL){
                printError("Syntax: '<s_label> -> label <id> ;' Expecting <id>", "<s_label>", pointer->next->value);
            }
            
            if (pointer->next->tokenClass != CLASS_LETTER){
                checkLabelToken = false;
                continue;
            }

            if (pointer->next->next != NULL && !checkLexeme(pointer->next->next, ";")){
                printError("Syntax: '<s_label> -> label <id> ;' Expecting ';'", "<s_label>", pointer->next->next->value);
            }

            addLabel(pointer->next,pointer->next->next);

        }else if (checkLexeme(pointer, ";")){
            checkLabelToken = true;
        }else{
            checkLabelToken = false;
        }
    }
}


/*
Compute Parse Function
    This parsing will compute the result.
    Argument:
        pointer - the starting point
*/
void computeParse(struct Lexeme * pointer){
    // <program> -> <statement> { ; <statement> } [;]
    while ((pointer->next) != NULL){
        pointer = symbolStatement(pointer->next);
        if (pointer == NULL){
            break;
        }else if (!checkLexeme(pointer, ";")){
            printError("Syntax: Statement CANNOT Be Resolved.", "<s_label>", pointer->value);
        }
    }
}


/*========================================*/
/*                  MAIN                  */
/*========================================*/

//Main Function
int main(){
    initialize();
    readSource(NULL);
    labelParse(lexemeList);
    computeParse(lexemeList);
    return 0;
}
