#include "PTemp.h"

// Global Variables
symbol symbol_Table[MAX_SYMBOL_TABLE_SIZE];     // Creates the symbol table as a global array
lexeme *lex_Array;
instruction VM_Instructions[MAX_SYMBOL_TABLE_SIZE];

int symbol_Table_Size = 0;
int lex_Array_Size = 0;
int VM_Instruct_Size = 0;
int prodecureCount = 0;

int token = -1;     // Token is used to get the next token in the token list

// HELPER FUNCTIONS

void Emit(int OP_Code, int L, int M) {
    instruction newInstruction;         // Creates a new instruction of type instructon
    newInstruction.opcode = OP_Code;    // Assigns the given opcode to the new opcode
    newInstruction.l = L;               // Assigns the given l to the new l
    newInstruction.m = M;               // Assigns the given m to the new m

    VM_Instructions[VM_Instruct_Size++] = newInstruction;       // Adds the new Instruction to Virtual Machine Instructions and increments VM Size
}

void Add_To_SymTable(int kind, char *name, int number, int level, int address, int mark, int param) {
    symbol newSybol;                // Creates a new Sybol
    newSybol.kind = kind;
    strcpy(newSybol.name, name);
    newSybol.val = number;
    newSybol.level = level;
    newSybol.addr = address;
    newSybol.mark = mark;
    newSybol.param = param;

    symbol_Table[symbol_Table_Size++] = newSybol;   // Adds the new symbol to the symbol table and increments the symbol table size
}

int Get_Token() {
    int token = lex_Array[lex_Array_Size++].tokenType;  // Retrieves the token from the lex Array. Increments the lex_Array_Size also.
    return token;                                       // Returns token
}

int Find_Procedure(int prodecureIndex) {
    for(int i = 0; i < symbol_Table_Size; i++) {
        if(symbol_Table[i].kind == 3 && symbol_Table[i].val == prodecureIndex) return i;
    }
    return -1;
}

void Mark(int count) {
    for(int i = symbol_Table_Size; i != -1; i--) {
        if(symbol_Table[i].mark == 0) {
            symbol_Table[i].mark = 1;
            count--;
        } else {
            continue;
        }
    }
}

int Symbol_Table_Check(char *checkName, int checkLevel) {
    for(int i = 0; i < symbol_Table_Size; i++) {

        // Checks the level, name, and mark status of each entry in the symbol table. If the requirements are met, the index is returned.
        if(symbol_Table[i].level == checkLevel && strcmp(symbol_Table[i].name, checkName) == 0 && symbol_Table[i].mark == 0) return i;
    }
    return -1;
}


int Const_Declaration(int lex_Level) {
    int num_Constants = 0;
    token = Get_Token();    // Gets the next token

    if(token == 28) {   // If the token is a Constant
        do {
            num_Constants++;    // Increases the number of constants
            token = Get_Token();    // Gets the next token

            if(token != 2) {    // Checks to see if there is an identifier after the constant, if not then an error is thrown.
                printf("Error: const, var, and read keywords must be followed by identifier\n");
                exit(-1);
            }

            if(Symbol_Table_Check(token, lex_Level) != -1){
                printf("symbol has already been declared\n");
                exit(-1);
            }

            int name_index = lex_Array_Size - 1;

            token = Get_Token();

            if(token != 9){
                printf("constants must be assigned with =\n");
                exit(-1);
            }

            token = Get_Token();

            if(toke != 3){
                printf("constants must be assigned an integer value\n");
                exit(-1);
            }

            Add_To_SymTable(1, lexeme_Array[name_index].lexeme, num_Constants, lex_Level, 0, 0, 0);

            token = Get_Token();

        } while (token == 17);

        if(token != 18){
            printf("constant and variable declarations must be followed by a semicolon\n");
            exit(-1);
        }
        token = Get_Token();

    }
    return -1;
}

int Variable_Declaration(int lex_Level, int param) {
    int numVars;
    token = Get_Token();

    if(param == 1)
        numVars = 1;
    else
        numVars = 0;

    if(token == 29) {
        do{
            numVars++;
            token = Get_Token();

            if(token != 2){
                printf("Error: const, var, and read keywords must be followed by identifier\n");
                exit(-1);
            }

            if(Symbol_Table_Check(token, lex_Level) != -1)
            {
                printf("symbol has already been declared\n");
                exit(-1);
            }

            int name_index = lex_Array_Size - 1;

            Add_To_SymTable(2, lexeme_Array[name_index].lexeme, 0, lex_Level, numVars + 3, 0, 0);
            token = Get_Token();
        } while(token == 17);

      if(token != 18) {
          printf("constant and variable declarations must be followed by a semicolon\n");
          exit(-1);
      }

      token = Get_Token();
      return numVars;
    }
}

void Block(int lexLevel, int param, int prodedureIndex) {
    int c = Const_Declaration(lexLevel);
    int v = Variable_Declaration(lexLevel);
    int p = Procedure_Declaration(lexLevel);
    symbol_Table[prodedureIndex].addr = VM_Instruct_Size;   // COULD BE WRONG
    emit(6, 0, 4 + v);  // Emits INC, Increases by 4 plus the number of variables

    Statement(lexLevel);

    Mark(c + v + p);
}

void Program() {
    int num_Procedures = 1;
    Emit(7, 0, 10);         // Create a filler JMP, the M value is not important right now

    for(int i = 0; i < sizeof(lex_Array) / sizeof(lex_Array[0]); i++) {     // Loops through the entire Lex_Array
        if(lex_Array[i].tokenType == 30) {  // Checks to see if the tokentype is a Procedure
            num_Procedures++;
            Emit(7, 0, 10);     // Another filler JMP, with a random M
        }
    }
    Add_To_SymTable(3, "main", 0, 0, 0, 0, 0);  // Adds the MAIN function to the symbol Table
    prodecureCount++;

    //Block(0, 0, 0);

    if(token != 19) {
        printf("Error: program must end with period\n");
        exit(-1);
    }

    for(int i = 0; i < num_Procedures; i++) {
        VM_Instructions[i].m = symbol_Table[Find_Procedure(i)].addr;
    }

    Emit(9, 0, 3);      // HALT
}

void ParserCodegenMain(lexeme *Lex_Token_Array, int A_Directive) {
    lex_Array = Lex_Token_Array;    // Copies the Lexeme Token Array into the Lex Array

}
