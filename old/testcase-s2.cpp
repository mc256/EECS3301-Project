#include "testcase-s2.hpp"

using namespace std;

// Main function
int main(){
    lexicalAnalysis();
    // Print out all the tokens
    struct Token * tokenPointer = head;
    while (tokenPointer->next != NULL){
        tokenPointer = tokenPointer->next;
        cout << tokenPointer->tokenClass << "\t" << tokenPointer->tokenCode << "\t" << tokenPointer->value.str() << endl;
    }
}