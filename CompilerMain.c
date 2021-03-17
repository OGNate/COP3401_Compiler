#include "CompilerMain.h"

/*
    To-Do:
        2) Create a general assembly print for the Parser and CodeGen
        3) Work on creating and using the different type of flags for priting information
        4) Pass the Code gen to the VM

*/

int main(int argc, char *argv[]) {

    int L_Directive = 0, A_Directive = 0, V_Directive = 0;

    for(int i = 2; i < argc; i++) {
        if(strcmp(argv[i], "-l") == 0) L_Directive = 1;
        if(strcmp(argv[i], "-a") == 0) A_Directive = 1;
        if(strcmp(argv[i], "-v") == 0) V_Directive = 1;
        if(L_Directive == 1 && A_Directive == 1 && V_Directive == 1) break;
    }

    printf("%d %d %d\n", L_Directive, A_Directive, V_Directive);


    // Passes the number of arguments and what arguments were given. LexMain runs the entire Lexical Analyzer 
    // which tokenizes every valid symbol, digit, and alpha in the given text. If it runs without error, it will
    // return the lexeme array which will then be used by the Parser in the next step of the compiler process.
    lexeme *Lexical_Tokens_Array = LexMain(argc, argv);    
    int lexeme_Size = 0;

    instruction *VM_Instructions = ParserCodegenMain(Lexical_Tokens_Array); // Runs the Parser and Codegen 

    VM_Main(VM_Instructions);   // Runs the Virtual Machine


    printf("Completed to here");
}