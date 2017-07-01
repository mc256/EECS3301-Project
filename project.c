#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


/*********************************
    Data Type and Variable
 **********************************/
#define MAX_LEXEME_LENGTH 100

#define CLASS_OTHERS 0
#define CLASS_LETTER 1
#define CLASS_DIGIT 2
#define CLASS_SYMBOL 3

#define CODE_EQUAL 1
#define CODE_ADD 2
#define CODE_MINUS 3
#define CODE_MULTIPLICATION 4
#define CODE_SEMICOLON 5

#define CODE_S_PRINT 11
#define CODE_S_LABEL 12
#define CODE_S_GOTO 13
#define CODE_S_IFPOS 14

struct Token{
    int tokenClass;
    char * value;
    size_t length;
    struct Token * next;
};

struct Token * tokenList;

struct Label {
    struct Token * labelName;
    struct Token * nextCommand;
    struct Label * next;
};

struct Label * labelList;

struct Variable {
    char * variableName;
    long variableValue;
    struct Variable * next;
};

struct Variable * variableList;

struct Token * symbolFactor(struct Token * pointer, long * writeBack);
struct Token * symbolTerm(struct Token * pointer, long * writeBack);
struct Token * symbolExpr(struct Token * pointer, long * writeBack);

/*********************************
    Utility
 **********************************/

void printError(char * error){
    printf("Error:\n\t%s\n", error);
    exit(1);
}

void initialize(){
    labelList = malloc(sizeof(struct Label));
    variableList = malloc(sizeof(struct Variable));
    if (labelList == NULL || variableList == NULL){
        printError("Not Enough Memory");
    }
    labelList->next = NULL;
    variableList->next = NULL;
}

/*********************************
    Buffer Operation
 **********************************/
char * createNewLexeme(){
    char * temp = malloc(MAX_LEXEME_LENGTH);
    if (temp == NULL){
        printError("Not Enough Memory");
    }
    return temp;
}

struct Token * createNewToken(){
    struct Token * temp = malloc(sizeof(struct Token));
    if (temp == NULL){
        printError("Not Enough Memory");
    }
    temp->length = 0;
    temp->value = createNewLexeme();
    return temp;
}

void readSource(struct Token * pointer){
    if (pointer == NULL){
        pointer = createNewToken();
        tokenList = pointer;
    }

    bool appendLexeme = false;
    char c;
    while ((c = getchar()) != EOF){
        if (isdigit(c)) {
            if (!appendLexeme){
                pointer->next = createNewToken();
                pointer = pointer->next;
                pointer->tokenClass = CLASS_DIGIT;
                appendLexeme = true;
            }            
            pointer->value[pointer->length ++] = c;
            pointer->value[pointer->length] = '\0';
        }else if (isalpha(c)) {
            if (!appendLexeme){
                pointer->next = createNewToken();
                pointer = pointer->next;
                pointer->tokenClass = CLASS_LETTER;
                appendLexeme = true;
            }            
            pointer->value[pointer->length ++] = c;
            pointer->value[pointer->length] = '\0';
        }else if (ispunct(c)){
            pointer->next = createNewToken();
            pointer = pointer->next;
            pointer->tokenClass = CLASS_SYMBOL;
            pointer->value[pointer->length ++] = c;
            pointer->value[pointer->length] = '\0';
        }else{
            appendLexeme = false;
            continue;
        }
    }

}

/*********************************
    Label
 **********************************/
struct Label * createNewLabel(){
    struct Label * temp = malloc(sizeof(struct Label *));
    if (temp == NULL){
        printError("Not Enough Memory");
    }
    temp->next = NULL;
    return temp;
}

void addLabel(struct Token * nameToken, struct Token * nextToken){
    //second declaration for the same label name will over write the previous one
    struct Label * p = labelList;
    while (p->next != NULL){
        p = p->next;
        if (strcmp(nameToken->value, p->labelName->value) == 0){
            p->nextCommand = nextToken;
            return;
        }
    }
    p->next = createNewLabel();
    p = p->next;
    
    p->labelName = nameToken;
    p->nextCommand = nextToken;
}

struct Token * gotoLabel(char * name){
    struct Label * p = labelList;
    while ((p = p->next) != NULL){
        if (strcmp(name, p->labelName->value) == 0){
            return p->nextCommand;
        }
    }
    printError("Lable Not Found");
    return NULL;
}

/*********************************
    Variable
 **********************************/
struct Variable * createNewVariable(){
    struct Variable * temp = malloc(sizeof(struct Variable));
    if (temp == NULL){
        printError("Not Enough Memory");
    }
    temp->next = NULL;
    return temp;
}

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

long getVariable(char * name){
    struct Variable * p = variableList;
    while ((p = p->next) != NULL){
        if (strcmp(name, p->variableName) == 0)   {
            return p->variableValue;
        }
    }
    printError("Variable Not Defined");
    return 0;
}

/*********************************
    Lexical Analysis
 **********************************/
bool checkLexeme(struct Token * token, char * compareTo){
    return strcmp(token->value, compareTo) == 0;
}

char * getLexeme(struct Token * token){
    return token->value;
}



/*********************************
    Language Support Functions
 **********************************/

struct Token * symbolFactor(struct Token * pointer, long * writeBack){
    // <factor> -> id | int_constant | (<expr>)
    long value = 0;
    if (checkLexeme(pointer, "(")){
        long cb = 0;
        pointer = symbolExpr(pointer->next, &cb);
        value = value + cb;
        if (!checkLexeme(pointer, ")")){
            printError("missing right round bracket");
        }
        * writeBack = value;
    }else if (pointer->tokenClass == CLASS_LETTER){
        value = getVariable(pointer->value);
    }else if (pointer->tokenClass == CLASS_DIGIT) {
        value = atol(pointer->value);
    }
    *writeBack = value;
    return pointer->next;
}

struct Token * symbolTerm(struct Token * pointer, long * writeBack){
    // <term> -> <factor> { * <factor>}
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

struct Token * symbolExpr(struct Token * pointer, long * writeBack){
    // <expr> -> <term> { ( + | - ) <term> } 
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



struct Token * statementPrint(struct Token * pointer){
    if (!checkLexeme(pointer, "print")){
        return pointer;    
    }

    if (pointer->next == NULL || (pointer->next->tokenClass != CLASS_DIGIT && pointer->next->tokenClass != CLASS_LETTER)) {
        return pointer;
    }

    long value = 0;
    pointer = symbolExpr(pointer->next, &value);
    printf("%ld\n", value);

    if (!checkLexeme(pointer, ";")){
        printError("Missing ';'");
    }
    return pointer;    
}

struct Token * statementGoTo(struct Token * pointer){
    if (!checkLexeme(pointer, "goto")){
        return pointer;
    }

    if (pointer->next == NULL || pointer->next->tokenClass != CLASS_LETTER) {
        return pointer;
    }

    return gotoLabel(pointer->next->value);
}


struct Token * statementIfPositive(struct Token * pointer){
    if (!checkLexeme(pointer, "ifpos")){
        return pointer;
    }

    if (pointer->next == NULL || (pointer->next->tokenClass != CLASS_DIGIT && pointer->next->tokenClass != CLASS_LETTER)) {
        return pointer;
    }

    long value = 0;
    pointer = symbolExpr(pointer->next, &value);
    //printf("%ld\n", value);

    if (!checkLexeme(pointer, "goto")){
        printError("missing goto");
    }

    if (pointer->next == NULL || pointer->next->tokenClass != CLASS_LETTER) {
        printError("missing goto target");
    }

    if (value > 0){
        return gotoLabel(pointer->next->value);
    }

    if (!checkLexeme(pointer->next->next, ";")){
        printError("Missing ';'");
    }   

    return pointer->next->next;
}

struct Token * statementAssign(struct Token * pointer){
    if (pointer->tokenClass != CLASS_LETTER){
        return pointer;
    }

    char * name = pointer->value;

    if (pointer->next == NULL || !checkLexeme(pointer->next, "=")){
        return pointer;
    }


    long value = 0;
    pointer = symbolExpr(pointer->next->next, &value);

    setVariable(name, value);

    if (!checkLexeme(pointer, ";")){
        printError("Missing ';'");
    }

    return pointer;
}

struct Token * statementIgnore(struct Token * pointer){
    while (pointer != NULL && !checkLexeme(pointer, ";")) {
        pointer = pointer->next;
    }
    return pointer;
}

/*********************************
    Parsing
 **********************************/

void labelParse(struct Token * pointer){
    bool checkLabelToken = true;
    while ((pointer = pointer->next) != NULL){
        if (checkLexeme(pointer, "label")){
            // Check the next two Tokens
            // <id>;
            if (pointer->next == NULL){
                printError("Syntax: <s_label> -> label <id> ; Expecting <id>");
            }
            
            if (pointer->next->tokenClass != CLASS_LETTER){
                checkLabelToken = false;
                continue;
            }

            if (pointer->next->next == NULL || !checkLexeme(pointer->next->next, ";")){
                printError("Syntax: <s_label> -> label <id> ; Expecting ';'");
            }

            addLabel(pointer->next,pointer->next->next);

        }else if (checkLexeme(pointer, ";")){
            checkLabelToken = true;
        }else{
            checkLabelToken = false;
        }
    }
}


void computeParse(struct Token * pointer){
    while ((pointer->next) != NULL){
        pointer = pointer->next;
        struct Token * cp = pointer;
        if ((pointer = statementPrint(pointer)) != cp){
            continue;
        }
        if ((pointer = statementGoTo(pointer)) != cp){
            continue;
        }
        if ((pointer = statementIfPositive(pointer)) != cp){
            continue;
        }
        if ((pointer = statementAssign(pointer)) != cp){
            continue;
        }
        if ((pointer = statementIgnore(pointer)) != cp){
            continue;
        }
        if (pointer == NULL){
            break;
        }
    }
}


//Test Cases
void testParse(){
    struct Token * p = tokenList;
    while ( (p=p->next) != NULL){
        printf("%s<------[%ld]\n", getLexeme(p), p->length);
        if (checkLexeme(p, "print")){
            printf("this is print\n");
        }
    }
    
    struct Label * q = labelList;
    while ( (q = q->next) != NULL ){
        printf("%s\n", q->labelName->value);
    }
    gotoLabel("this");


    setVariable("33123aa", 123);
    setVariable("33123", 555);
    setVariable("33123", 56566);

    struct Variable * r = variableList;
    while ( (r = r->next) != NULL ){
        printf("%s=%ld\n", r->variableName, r->variableValue);
    }

    long number;
    number = getVariable("33123aa");
    printf("-->%ld\n", number);
    number = getVariable("33123");
    printf("-->%ld\n", number);

    printf("%ld\n", atol("6478702268"));
}

//Main Function
int main(){
    initialize();
    readSource(NULL);
    labelParse(tokenList);
    //testParse();
    computeParse(tokenList);

    return 0;
}