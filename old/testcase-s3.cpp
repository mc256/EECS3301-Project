#include "testcase-s3.hpp"

using namespace std;


// Main function
int main(){
    struct Token * lastToken = lexicalAnalysis(NULL);
    // Print out all the tokens
    struct Token * tokenPointer = head;
    while (tokenPointer->next != NULL){
        tokenPointer = tokenPointer->next;
        cout << tokenPointer->tokenClass << "\t" << tokenPointer->tokenCode << "\t" << tokenPointer->value.str() << endl;
    }

    cout << "The last token is:" << endl << lastToken->value.str() << endl;
    cout << "=================================" << endl;

    // Check labels
    struct Label * lastLabel = checkLabel(labelList, head);
    struct Label * labelPointer = labelList;
    // Start Execution
    while (labelPointer->next != NULL){
        labelPointer = labelPointer->next;
        cout << (* labelPointer->variableName).str() << endl;
    }

}