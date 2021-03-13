#include "CompilerMain.h"

/*
    To-Do:
        2) Create a general assembly print for the Parser and CodeGen
        3) Work on creating and using the different type of flags for priting information
        4) Pass the Code gen to the VM

*/

int main(int argc, char *argv[]) {
    // Passes the number of arguments and what arguments were given. LexMain runs the entire Lexical Analyzer 
    // which tokenizes every valid symbol, digit, and alpha in the given text. If it runs without error, it will
    // return the lexeme array which will then be used by the Parser in the next step of the compiler process.
    lexeme *Lexical_Tokens_Array = LexMain(argc, argv); 
    int lexeme_Size = 0;

    instruction *VM_Instructions = ParserCodegenMain(Lexical_Tokens_Array);

    int i = 0;
    while(VM_Instructions[i].opcode != 0) { // WE check for 0 since there will never be a 0 in the opcode spot
        printf("%d %d %d\n", VM_Instructions[i].opcode, VM_Instructions[i].l, VM_Instructions[i].m);
        i++;
    }


    printf("Completed to here");
}