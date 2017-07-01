#include "s2.hpp"

using namespace std;

// Check character type
bool isLetter(char c){
    if ((c >= 'A' && c <= 'Z')||(c >= 'a' && c <= 'z')){
        return true;
    }
    return false;
}

bool isDigit(char c){
    if (c >= '0' && c <= '9'){
        return true;
    }
    return false;
}

bool isSymbol(char c){
    if (c == '+' || c == '-' || c == '*' || c == '=' || c == ';'){
        return true;
    }
    return false;
}

bool isEmpty(char c){
    if (c == '\r' || c == '\n' || c == ' ' || c == '\t'){
        return true;
    }
    return false;
}

struct Token * lexicalAnalysis(struct Token * currentToken){
    if (currentToken == NULL){
        currentToken = new Token;
        head = currentToken;
    }

    bool appendLexeme = false;
    char c;

    while (cin.get(c)){
        if (isDigit(c)){
            if (appendLexeme == false){
                currentToken->next = new Token;
                currentToken = currentToken->next;
                currentToken->tokenClass = CLASS_DIGIT;
                currentToken->value << c;
                appendLexeme = true;
            }else{
                currentToken->value << c;
            }
        }else if (isLetter(c)){
            if (appendLexeme == false){
                currentToken->next = new Token;
                currentToken = currentToken->next;
                currentToken->tokenClass = CLASS_LETTER;
                currentToken->value << c;
                appendLexeme = true;
            }else{
                currentToken->value << c;
            }
        }else if (isSymbol(c)){
            currentToken->next = new Token;
            currentToken = currentToken->next;
            currentToken->tokenClass = CLASS_SYMBOL;
            currentToken->value << c;
            appendLexeme = true;
        }else if (isEmpty(c)){
            appendLexeme = false;
            continue;
        }else{
            // This can be comments
            if (appendLexeme == false){
                currentToken->next = new Token;
                currentToken = currentToken->next;
                currentToken->tokenClass = CLASS_OTHERS;
                currentToken->value << c;
                appendLexeme = true;
            }else{
                currentToken->value << c;
            }
        }
    }

    //Go through the list again and assign the token codes
    currentToken = head;
    while (currentToken->next != NULL){
        currentToken = currentToken->next;
        if (currentToken->tokenClass == CLASS_SYMBOL){
            if (currentToken->value.str() == "="){
                currentToken->tokenCode = CODE_EQUAL;
            }else if (currentToken->value.str() == "+"){
                currentToken->tokenCode = CODE_ADD;
            }else if (currentToken->value.str() == "-"){
                currentToken->tokenCode = CODE_MINUS;
            }else if (currentToken->value.str() == "*"){
                currentToken->tokenCode = CODE_MULTIPLICATION;
            }else if (currentToken->value.str() == ";"){
                currentToken->tokenCode = CODE_SEMICOLON;
            }
        }else if (currentToken->tokenClass == CLASS_LETTER){
            if (currentToken->value.str() == "print"){
                currentToken->tokenCode = CODE_S_PRINT;
            }else if (currentToken->value.str() == "label"){
                currentToken->tokenCode = CODE_S_LABEL;
            }else if (currentToken->value.str() == "goto"){
                currentToken->tokenCode = CODE_S_GOTO;
            }else if (currentToken->value.str() == "ifpos"){
                currentToken->tokenCode = CODE_S_IFPOS;
            }
        }
    }

    return currentToken;
}
