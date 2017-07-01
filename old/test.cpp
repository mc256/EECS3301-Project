#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(){

    char c;
    while (cin.get(c)){
        if (c == EOF){
            cout << "EOF" << endl;
        }else if (c == ' '){
            cout << "SPACE";
        }else if (c == '\n'){
            cout << "EOL[n]" << endl;
        }else if (c == '\r'){
            cout << "EOL[r]" << endl;
        }else{
            cout << c;
        }            
    }

    return 0;
}