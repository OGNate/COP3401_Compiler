#include "CompilerMain.h"

int main(int argc, char *argv[]) {

    int L_Directive = 0, A_Directive = 0, V_Directive = 0;

    if(argc < 2) {
        printf("Please enter an input file\n");
        exit(-1);
    }

    for(int i = 2; i < argc; i++) {
        if(strcmp(argv[i], "-l") == 0) L_Directive = 1;
        if(strcmp(argv[i], "-a") == 0) A_Directive = 1;
        if(strcmp(argv[i], "-v") == 0) V_Directive = 1;
        if(L_Directive == 1 && A_Directive == 1 && V_Directive == 1) break;
    }

    // Passes the number of arguments and what arguments were given. LexMain runs the entire Lexical Analyzer 
    // which tokenizes every valid symbol, digit, and alpha in the given text. If it runs without error, it will
    // return the lexeme array which will then be used by the Parser in the next step of the compiler process.
    lexeme *Lexical_Tokens_Array = LexMain(argc, argv, L_Directive);    
    int lexeme_Size = 0;
 
    // Passes the Lexical_Tokens_Array to the Parser and Code generator. This will parse the entire code and then generate the code
    // That will then be sent to the Virtual Machine
    instruction *VM_Instructions = ParserCodegenMain(Lexical_Tokens_Array, A_Directive); // Runs the Parser and Codegen 

    VM_Main(VM_Instructions, V_Directive);   // Runs the Virtual Machine
}