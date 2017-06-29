#include <iostream>
#include <sstream>
#include <string>

struct Variable {
    std::stringstream * variableName;
    int variableValue;
    struct Variable * next;
};

struct Label {
    std::stringstream * variableName;
    struct Token * labelToken; // Points to the semicolon of the label statement
    struct Label * next;
};

// External Variable
extern struct Token * executePointer;
extern struct Variable * variableList;
extern struct Label * labelList;

// Declare function
struct Token * execute(struct Token * startPoint);