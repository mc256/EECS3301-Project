#include "testcase-s3.hpp"

using namespace std;


// Label RDP
void findLabels(){

    tokenPointer = head;
    while (tokenPointer->next != NULL){
        tokenPointer = tokenPointer->next;
        if (tokenPointer->tokenCode == CODE_S_LABEL){

        }
    }
}

// Main function
int main(){
    lexicalAnalysis();
    // Print out all the tokens
    struct Token * tokenPointer = head;
    while (tokenPointer->next != NULL){
        tokenPointer = tokenPointer->next;
        cout << tokenPointer->tokenClass << "\t" << tokenPointer->tokenCode << "\t" << tokenPointer->value.str() << endl;
    }

    // Check labels
    // Start Execution


}