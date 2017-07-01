#include "s3.hpp"

using namespace std;

//<expr> -> <term> { ( + | - ) <term> } 
//<term> -> <factor> { * <factor>} //integer multiplication
//<factor> -> id | int_constant | (<expr>)
//statements and program
//<s_print> -> print <expr> ; //evaluates the expression <expr> and prints the result
//<s_assign> -> <id> = <expr> ; //assignment statement
//<s_label> -> label <id> ; //creates a label (handle it in a separate run of the parser)
//<s_goto> -> goto <id> ; //jumps to the label associated with <id>
//<s_ifpos> -> ifpos <expr> goto <id> ; //if <expr> > 0 jumps to the label <id>
//<statement> -> <s_print> |<s_assign> |<s_label> |<s_goto> |<s_ifpos>
//<program> -> <statement> { ; <statement> } ;

// Label RDP
struct Label * checkLabel(struct Label * currentLabel, struct Token * startingPoint) {
    if (currentLabel == NULL){
        currentLabel = new Label;
        labelList = currentLabel;
    }

    bool checkLabelTokenCode = true;
    struct Token * tokenPointer = startingPoint;
    while (tokenPointer->next != NULL) {
        tokenPointer = tokenPointer->next;
        if (checkLabelTokenCode && tokenPointer->tokenCode == CODE_S_LABEL) {
            // Check the next two tokens
            // <id>
            if (tokenPointer->next == NULL || tokenPointer->next->tokenClass != CLASS_LETTER) {
                // Throw error: expecting <id>
                cout << "Syntax Error: Expecting <id>" << endl << ">" << flush;
                return NULL;
            }
            // ;
            if (tokenPointer->next->next == NULL || tokenPointer->next->next->tokenCode != CODE_SEMICOLON) {
                // Throw error: expecting semicolon
                cout << "Syntax Error: Expecting semicolon" << endl << ">" << flush;
                return NULL;
            }

            // Add new label structure to the link list
            currentLabel->next = new Label;
            currentLabel = currentLabel->next;
            currentLabel->variableName = &(tokenPointer->next->value);
            currentLabel->labelToken = tokenPointer->next->next;
            
        }else if (tokenPointer->tokenCode == CODE_SEMICOLON){
            checkLabelTokenCode = true;
        }
    }


}

struct Token * execute(struct Token * startPoint) {

}
