#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "LexAnalyzer.h"

/*
    To-Do:
        1) Work on implimenting The Parser
        2) Work on implimenting the Code Generator
        3) Work on creating and using the different type of flags for priting information
        4) Pass the Code gen to the VM

*/

int main(int argc, char *argv[]) {
    // Passes the number of arguments and what arguments were given. LexMain runs the entire Lexical Analyzer 
    // which tokenizes every valid symbol, digit, and alpha in the given text. If it runs without error, it will
    // return the lexeme array which will then be used by the Parser in the next step of the compiler process.
    lexeme *Lexical_Tokens_Array = LexMain(argc, argv); 

    printf("Completed to here");
}