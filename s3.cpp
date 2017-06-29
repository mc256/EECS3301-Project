#include "s3.hpp"

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
