#include "ParserCodegen.h"

/*
    TO-DO:
        1) Not sure about the implimentation of the MARK function. I don't really understand how it works by looking at the pseudo code
            so if you want to take a look and play around with it have at it.
        2) I have not run the code yet so I'm not sure if it works. There are most likely bugs and errors in it that we have to fix
            but we can do that tomorrow (Saturday).

*/

// Global Variables
symbol symbol_Table[MAX_SYMBOL_TABLE_SIZE];     // Creates the symbol table as a global array
lexeme *lex_Array;            
instruction VM_Instructions[MAX_SYMBOL_TABLE_SIZE];                  

int symbol_Table_Size = 0;
int lex_Array_Size = 0;
int VM_Instruct_Size = 0;
int procedure_Count = 0;

int token = -1;     // Token is used to get the next token in the token list

// HELPER FUNCTIONS

void print_VM_Instructions() {
    for(int i = 0; i < VM_Instruct_Size; i++) {
        printf("%d %d %d\n", VM_Instructions[i].opcode, VM_Instructions[i].l, VM_Instructions[i].m);
    }
}

void print_Symbol_Table() {
    for(int i = 0; i < symbol_Table_Size; i++) printf("%d %s %d %d %d %d %d\n", symbol_Table[i].kind, symbol_Table[i].name, symbol_Table[i].val, symbol_Table[i].level, symbol_Table[i].addr, symbol_Table[i].mark, symbol_Table[i].param);
}

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

int Symbol_Table_Search(char *name, int lex_Level, int kind) {
    for(int i = 0; i < symbol_Table_Size; i++) {
        if(strcmp(symbol_Table[i].name, name) == 0 && symbol_Table[i].kind == kind) {
            return i;
        }
    }
    return -1;
}

char* get_OP_Name(int OP_Code) {
    char* op_Name = malloc(sizeof(char) * 4);
    switch(OP_Code){
        case 1:
            strcpy(op_Name, "LIT");
            break;

        case 2:
            strcpy(op_Name, "OPR");
            break;

        case 3:
            strcpy(op_Name, "LOD");
            break;

        case 4:
            strcpy(op_Name, "STO");
            break;

        case 5:
            strcpy(op_Name, "CAL");
            break;

        case 6:
            strcpy(op_Name, "INC");
            break;

        case 7:
            strcpy(op_Name, "JMP");
            break;

        case 8:
            strcpy(op_Name, "JPC");
            break;

        case 9:
            strcpy(op_Name, "SYS");
            break;

        default:
            break;
    }
    return op_Name;
}

void Print_General_Assembly() {
    printf("Generated Assembly:\n");
    printf("Line\tOP\tL\tM\n");

    for(int i = 0; i < VM_Instruct_Size; i++) {
        printf("%d\t%s\t%d\t%d\n", i, get_OP_Name(VM_Instructions[i].opcode), VM_Instructions[i].l, VM_Instructions[i].m);
    }
    printf("\n\n");
}

// ------------------------------------------------------------------------------------------------------------------------------------------
// Const_Declaration determines how many valid constants are in the given code. It will then return the amount to Block. 
// If there are errors in how a constant is declared, an error message will pop up and the program will stop.
int Const_Declaration(int lex_Level) {
    int num_Constants = 0;
    char *saved_Ident_Name;
    int save_Index = -1;

    //token = Get_Token();    // Gets the next token

    if(token == 28) {   // If the token is a Constant
        do {
            num_Constants++;    // Increases the number of constants
            token = Get_Token();    // Gets the next token

            if(token != 2) {    // Checks to see if there is an identifier after the constant, if not then an error is thrown.
                printf("Error: const, var, and read keywords must be followed by identifier\n");
                exit(-1);
            }

            // Checks to see if the symbol is already in the symbol table at the designated lex level
            if(Symbol_Table_Check(lex_Array[lex_Array_Size - 1].lexeme, lex_Level) != -1) {
                printf("Error: competing symbol declarations at the same level\n");
                exit(-1);
            }

            save_Index = lex_Array_Size - 1;    // Saves the index of the constant identifier 

            token = Get_Token();    // Gets next token

            if(token != 9) {
                printf("Error: constants must be assigned with =\n");
                exit(-1);
            }

            token = Get_Token();    // Gets next token

            if(token != 3) {
                printf("Error: constants must be assigned an integer value\n");
                exit(-1);
            }

            // Adds the constant to the symbol table. It converts the string number into an integer value using atoi
            Add_To_SymTable(1, lex_Array[save_Index].lexeme, atoi(lex_Array[lex_Array_Size - 1].lexeme), lex_Level, 0, 0, 0);

            token = Get_Token();    // Gets next token

        } while (token == 17);  // While there are ","s it will continue to run through this loop

        if(token != 18) {   // Checks to make sure there is a semicolon after the constant declarations
            printf("Error: constant and variable declarations must be followed by a semicolon\n");
            exit(-1);
        }

        token = Get_Token();    // Gets next token
    }
    return num_Constants;      // Returns the number of constants
}

// --------------------------------------------------------------------------------------------------------------------------------------
// Variable Declarations counts how many variables are declared (given that there are no errors) and returns this amount. It will also
// add the variables to the symbol table.
int Variable_Declaration(int lex_Level, int param) {
    int num_Vars;

    // This if-else statement checks to see if there are any variables parameters passed, if so, then the count
    // of num_Vars is incremented to 1.
    if(param == 1) num_Vars = 1;
    else num_Vars = 0;

    if(token == 29) {   // Checks to see if the token is a variable
        do {    // Loops while there are commas
            num_Vars++; // Increments the number of variables by one

            token = Get_Token();    // Gets next token

            if(token != 2) {    // Checks to see if the next token is an identifier
                printf("Error: const, var, procedure, call, and read keywords must be followed by identifier\n");
                exit(-1);
            }

            // Checks the symbol table for the name of the identifier and at what lexigraphical level. If it returns -1, 
            // the variable identifier name has not been used and it will NOT throw and error.
            if(Symbol_Table_Check(lex_Array[lex_Array_Size - 1].lexeme, lex_Level) != -1) {
                printf("Error: competing symbol declarations at the same level\n");
                exit(-1);
            }

            // Adds the variable to the symbol table
            Add_To_SymTable(2, lex_Array[lex_Array_Size - 1].lexeme, 0, lex_Level, num_Vars + 3, 0, 0);

            token = Get_Token();    // Get next token

        } while(token == 17); // Checks to see if the next token is a comma ","

        if(token != 18) {   // Checks to make sure there is a semicolon ";" after the variable declarations
            printf("Error: constant and variable declarations must be followed by a semicolon\n");
            exit(-1);
        }

        token = Get_Token();    // Gets next token
    }
    return num_Vars;    // Returns the number of variables
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Procedure Declaration takes care of the procdures and adding them to the symbol table.
int Procedure_Declaration(int lex_Level) {
    int num_Procedures = 0;
    int procedure_Index;

    if(token == 30) {   // Checks to see if the token is a procedure
        do {    // Loops once
            num_Procedures++;   // Increments the count of the number of procedures

            token = Get_Token();    // Gets next token

            if(token != 2) {    // Checks to see if the token following the procedure keyword is an identifier
                printf("Error: const, var, procedure, call, and read keywords must be followed by identifer\n");
                exit(-1);
            }

            // Checks to see if the procedure has already been created and added to the symbol table
            if(Symbol_Table_Check(lex_Array[lex_Array_Size - 1].lexeme, lex_Level) != -1) {
                printf("Error: competing symbol declarations at the same level\n");
                exit(-1);
            }

            procedure_Index = symbol_Table_Size;    // Assigns the procedure index the index of the last element in the symbol table

            // Adds the procedure to the symbol table
            Add_To_SymTable(3, lex_Array[lex_Array_Size - 1].lexeme, procedure_Count, lex_Level, 0, 0, 0);

            procedure_Count++;  // Increments the procedure count

            token = Get_Token();    // Gets next token

            if(token == 15) {   // Checks to see if the token is a left parenthesis "("
                token = Get_Token();    // Gets next token

                if(token != 2) {    // Checks if the token is an identifier
                    printf("Error: parameters may only be specified by an identifier\n");
                    exit(-1);
                }

                // Adds the procedure parameter to the symbol table. It increments the lexigraphical level by one since we are going
                // into a procedure.
                Add_To_SymTable(2, lex_Array[lex_Array_Size - 1].lexeme, 0, lex_Level + 1, 3, 0, 0);

                symbol_Table[procedure_Index].param = 1;    // Assigns the parameter flag for the procedure int he symbol table to 1 since it has a parameter

                token = Get_Token();    // Gets next token

                if(token != 16) {   // Checks to make sure the next token is the right parenthesis ")"
                    printf("Error: right parenthesis must follow left parenthsis\n");
                    exit(-1);
                }

                token = Get_Token();    // Gets next token

                if(token != 18) {   // Checks to make sure the next token is a semicolon ";"
                    printf("Error: symbol declarations must be followed by a semicolon\n");
                    exit(-1);
                }

                token = Get_Token();    // Gets next token

                Block(lex_Level + 1, 1, procedure_Index);   // Calls the block function
            } else {
                if(token != 18) {   // Checks to make sure the next token is a semicolon ";"
                    printf("Error: symbol declaration must be followed by a semicolon\n");
                    exit(-1);
                }

                token = Get_Token();    // Gets next token

                Block(lex_Level + 1, 0, procedure_Index);   // Calls the block function
            }

            if(VM_Instructions[VM_Instruct_Size - 1].opcode != 2 && VM_Instructions[VM_Instruct_Size - 1].m != 0) {
                Emit(1, 0, 0);  // Emits LIT to the Virtual Machine Instructions
                Emit(2, 0, 0);  // Emits RTN to the Virtual Machine Instructions
            }

            if(token != 18) {   // Cheks to make sure the next token is a semicolon
                printf("Error: symbol declaration must be followed by a semicolon\n");
                exit(-1);
            }

            token = Get_Token();    // Gets next token

        } while(token == 30);   // Will continue looping while the token is a procedure
    }
    return num_Procedures;  // Returns the number of procedures
}

// -----------------------------------------------------------------------------------------------------------------------------------------
// Statement handles the conditions of certain keywords and how to handle each individual one.
void Statement(int lex_Level) {
    if(token == 2) {    // Checks to see if the token is an identifier
        int symbol_Index = Symbol_Table_Search(lex_Array[lex_Array_Size - 1].lexeme, lex_Level, 2); // Returns the symbol index if it is in the symbol table

        if(symbol_Index == -1) {    // Checks the symbol index, if it is equal to -1, then it was not found in the symbol table
            printf("Error: undeclared variable or constant in equation HERE\n");
            exit(-1);
        }

        token = Get_Token();    // Gets next token

        if(token != 20) {   // Checks to see if the next token is a walrus ":="
            printf("Error: assignment statements must use :=\n");
            exit(-1);
        }

        token = Get_Token();    // Gets next token

        Expression(lex_Level);

        Emit(4, lex_Level - symbol_Table[symbol_Index].level, symbol_Table[symbol_Index].addr); // Emits STO to the Virtual Machine Instructions

        return;
    }

    if(token == 27) {   // Checks to see if the token is the "call" keyword
        token = Get_Token();    // Gets next token

        if(token != 2) {    // Checks to see if the next token is an identifier
            printf("Error: const, var, procedure, call, and read keywords must be followed by identifier\n");
            exit(-1);
        }

        // Checks the symbol table for the specified procedure
        int symbol_Index = Symbol_Table_Search(lex_Array[lex_Array_Size - 1].lexeme, lex_Level, 3);

        if(symbol_Index == -1) {    // If the procedure was not found in the symbol table, this error will throw.
            printf("Error: undeclared procedure for call");
            exit(-1);
        }

        token = Get_Token();    // Gets next token

        if(token == 15) {   // Checks to see if the next token is the left parenthesis "("
            token  = Get_Token();   // Gets next token

            if(symbol_Table[symbol_Index].param != 1) {
                printf("Error: parameters must be declared\n");
                exit(-1);
            }

            Expression(lex_Level);  // calls Expression

            if(token != 16) {   // Checks to see if there is a matching right parenthesis to the left parenthesis
                printf("Error: right parenthesis must follow left parenthesis\n");
                exit(-1);
            }

            token = Get_Token();    // Gets next token

        } else {
            Emit(1, 0, 0);  // Emits LIT to the Virtual Machine
        }

        Emit(5, lex_Level - symbol_Table[symbol_Index].level, symbol_Table[symbol_Index].val);  // Emits CAL to the Virtual Machine Instructions

        return;
    }

    if(token == 34) {   // Checks to see if the next token is the "return" keyword
        if(lex_Level == 0) {    // Checks to see if the code is already in the main function, if so it cannot return
            printf("Error: cannot return from main\n");
            exit(-1);
        }

        token = Get_Token();    // Gets next token

        if(token == 15) {   // Checks to see if the next token is a left parenthesis "("
            token = Get_Token();    // Gets next token

            Expression(lex_Level);  // calls Expression

            Emit(2, 0, 0);  // Emits RTN to the Virtual Machine Instructions

            if(token != 16) {   // Checks to see if there is a right parenthesis following the left parenthesis
                printf("Error: right parenthesis must follow left parenthesis\n");
                exit(-1);
            }

            token = Get_Token();

        } else {
            Emit(1, 0, 0);  // Emits LIT to the Virtual Machine Instructions
            Emit(2, 0, 0);  // Emits RTN to the Virtual Machine Instructions
        }
        return;
    }

    if(token == 21) {   // Checks to see if the next token is the "begin" keyword
        do {
            token = Get_Token();    // Gets next token
            Statement(lex_Level);   // Calls Statement
        } while(token == 18);   // Loops while token is equal to the semicolon ";"

        if(token != 22) {   // Checks to make sure there is eventually an "end" keyword after the "begin" keyword
            printf("Error: begin must be followed by end\n");
            exit(-1);
        }

        token = Get_Token();    // Gets next token
        return;
    }

    if(token == 23) {   // Checks to see if the next token is the "if" keyword
        token = Get_Token();    // Gets next token

        Condition(lex_Level);   // Calls Condition

        int jpc_Index = VM_Instruct_Size;   

        Emit(8, 0, 0);  // Emits JPC to the Virtual Machine Instructions

        if(token != 24) {   // Checks to make sure there is a "then" keyword following the "if" keyword
            printf("Error: if must be followed by then\n");
            exit(-1);
        }

        token = Get_Token();    // Gets next token

        Statement(lex_Level);   // Calls Statement

        if(token == 33) {   // Checks if the next token is the "else" keyword
            token = Get_Token();    // Gets next token
            
            int jmp_Index = VM_Instruct_Size;   
            Emit(7, 0, 0);  // Emits JMP to the Virtual Machine Instructions

            VM_Instructions[jpc_Index].m = VM_Instruct_Size;    

            Statement(lex_Level);   // Calls Statement

            VM_Instructions[jmp_Index].m = VM_Instruct_Size;

        } else {
            VM_Instructions[jpc_Index].m = VM_Instruct_Size;
        }
        return;
    }

    if(token == 25) {   // Checks to see if the next token is the "while" keyword
        token = Get_Token();    // Gets next token

        int loop_Index = VM_Instruct_Size;

        Condition(lex_Level);

        if(token != 26) {   // Checks to see if the "do" keyword follows the "while" keyword
            printf("Error: while must be followed by do\n");
            exit(-1);
        }

        token = Get_Token();    // Gets next token

        int jpc_Index = VM_Instruct_Size;

        Emit(8, 0, 0);  // Emit JPC to the Virtual Machine Instructions

        Statement(lex_Level);

        Emit(7, 0, loop_Index); // Emits JMP to the Virtual Machine Instructions

        VM_Instructions[jpc_Index].m = VM_Instruct_Size;

        return;
    }

    if(token == 32) {   // Checks to see if the next token is the "read" keyword
        token = Get_Token();    // Gets next token

        if(token != 2) {    // Checks to see if the token following the "read" keyword is an identifier
            printf("Error: const, var, procedure, call, and read keywords must be followed by identifier\n");
            exit(-1);
        }

        int symbol_Index = Symbol_Table_Search(lex_Array[lex_Array_Size - 1].lexeme, lex_Level, 2); // Searches the symbol taable for the identifier

        if(symbol_Index == -1) {    // Checks to see if the identifier is in the symbol table
            printf("Error: undeclared variable or constant in equation\n");
            exit(-1);
        }

        token = Get_Token();    // Gets next token

        Emit(9, 0, 2);  // Emits READ (SYS) to the Virtual Machine Instructions
        Emit(4, lex_Level - symbol_Table[symbol_Index].level, symbol_Table[symbol_Index].addr); // Emits STO to the Virtual Machine Instructions
        return;
    }

    if(token == 31) { // Checks if the next token is the "write" keyword
        token = Get_Token();    // Gets next token
        Expression(lex_Level);  // Calls Expression
        Emit(9, 0, 1);  // Emits Write (SYS) to the Virtual Machine Instructions
        return;
    }
}

// ----------------------------------------------------------------------------------------------------------------------------------------
// Condition checks on the different types of conditional operators and then checks the expressions
void Condition(int lex_Level) {
    if(token == 8) {    // Checks whether the "odd" comparison is used
        token = Get_Token();    // Gets next token
        Expression(lex_Level);  // Calls Expression
        Emit(2, 0, 6);  // Emits ODD to the Virtual Machine Instructions

    } else {
        Expression(lex_Level);  // Calls Expression

        if(token == 9) { // Checks if the next token is the equals sign "="
            token = Get_Token();    // Gets next token
            Expression(lex_Level);  // Calls Expression
            Emit(2, 0, 8);  // Emits EQL to the Virtual Machine Instructions

        } else if(token == 10) { // Checks for "<>"
            token = Get_Token();    // Gets next token
            Expression(lex_Level);  // calls Expression
            Emit(2, 0, 9);  // Emits NEQ

        } else if(token == 11) {    // Checks for "<"
            token = Get_Token();    // Gets next token
            Expression(lex_Level);  // Calls Expression
            Emit(2, 0, 10); // Emits LSS

        } else if(token == 12) {    // Checks for "<="
            token = Get_Token();    // Gets next token
            Expression(lex_Level);  // Calls Expression
            Emit(2, 0, 11); // Emits LEQ

        } else if(token == 13) {    // Checks for ">"
            token = Get_Token();    // Gets next token
            Expression(lex_Level);  // Calls Expression
            Emit(2, 0, 12); // Emits GTR

        } else if(token == 14) {    // Checks for ">="
            token == Get_Token();   // Gets next token
            Expression(lex_Level);  // Calls Expression
            Emit(2, 0, 13); // Emits GEQ

        } else {
            printf("Error: condition must contain comparison operator\n");
            exit(-1);
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------------------------
void Expression(int lex_Level) {
    if(token == 5) { // Checks for "-"
        token = Get_Token();    // Gets next token
        Term(lex_Level);    // Calls Term
        Emit(2, 0, 1);  // Emits NEG

        while(token == 4 || token == 5) {   // Loop runs while token equals "+" or "-"
            if(token == 4) {    // Checks if the token is a "+"
                token = Get_Token();    // Gets next token
                Term(lex_Level);    // Calls Term
                Emit(2, 0, 2);  // Emits ADD

            } else {
                token = Get_Token();    // Gets next token
                Term(lex_Level);    // Calls Term
                Emit(2, 0, 3);  // Emits SUB to the Virtual Machine Instructions
            }
        }
    } else {
        if(token == 4) {    // Checks if the token is "+"
            token = Get_Token();    // Gets next token
        }

        Term(lex_Level);    // Calls Term

        while(token == 4 || token == 5) {   // Checks if the token is either "+" or "-"
            if(token == 4) {
                token = Get_Token();    // Gets next token
                Term(lex_Level);    // Calls term
                Emit(2, 0, 2);  // Emits ADD

            } else {
                token = Get_Token();    // Gets next token
                Term(lex_Level);    // Calls Term
                Emit(2, 0, 3);  // Emits SUB
            }
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------------------
void Term(int lex_Level) {
    Factor(lex_Level);  // Calls Factor

    while(token == 6 || token == 7 || token == 1) { // Checks if token is "*", "/", or "%"
        if(token == 6) {    // Checks if the token is "*"
            token = Get_Token();    // Gets next token
            Factor(lex_Level);  // Calls Factor
            Emit(2, 0, 4);  // Emits MUL

        } else if(token == 7) {     // Checks if the token is "/"
            token = Get_Token();    // Gets next token
            Factor(lex_Level);  // Calls Factor
            Emit(2, 0, 5);  // Emits DIV

        } else {    // Checks if the token is "%"
            token = Get_Token();    // Gets next token
            Factor(lex_Level);  // Calls Factor
            Emit(2, 0, 7);  // Emits MOD
        }
    }
} 

// -----------------------------------------------------------------------------------------------------------------------------------------
void Factor(int lex_Level) {
    if(token == 2) {
        int symbol_Index_V = Symbol_Table_Search(lex_Array[lex_Array_Size - 1].lexeme, lex_Level, 2);
        int symbol_Index_C = Symbol_Table_Search(lex_Array[lex_Array_Size - 1].lexeme, lex_Level, 1);

        if(symbol_Index_V == -1 && symbol_Index_C == -1) {  // Checks if the identifier (variable or constant) is not in the symbol table
            printf("Error: undeclared variable or constant in equation\n");
            exit(-1);

        } else if(symbol_Index_C == -1 || (symbol_Index_V != -1 && symbol_Table[symbol_Index_V].level > symbol_Table[symbol_Index_C].level)) {
            Emit(3, lex_Level - symbol_Table[symbol_Index_V].level, symbol_Table[symbol_Index_V].addr); // Emits LOD to the Virtual Machine

        } else {
            Emit(1, 0, symbol_Table[symbol_Index_C].val);   // Emits LIT to the Virtual Machine
        }
    } else if(token == 3) { // Checks for an integer number
        Emit(1, 0, atoi(lex_Array[lex_Array_Size - 1].lexeme));  // Emits LIT to the Virtual Machine
        token = Get_Token();    // Gets next token

    } else if(token == 15) {    // Checks for left parenthesis "("
        token = Get_Token();    // Gets next token
        Expression(lex_Level);  // Calls Expression
        
        if(token != 16) {   // Checks for the right parenthesis ")"
            printf("Error: right parenthesis must follow left parenthesis\n");
            exit(-1);
        }

        token = Get_Token();    // Gets next token

    } else if(token == 27) {    // Checks if the token is the keyword "call"
        Statement(lex_Level);   // Calls Statement

    } else {
        printf("Error: Arithmetic equations must contain operands, parentheses, numbers, or symbols\n");
        exit(-1);
    }
}

// -----------------------------------------------------------------------------------------------------------------------------------------
void Block(int lexLevel, int param, int prodedureIndex) {
    int c = Const_Declaration(lexLevel);
    int v = Variable_Declaration(lexLevel, param);
    int p = Procedure_Declaration(lexLevel);
    symbol_Table[prodedureIndex].addr = VM_Instruct_Size;   // COULD BE WRONG
    Emit(6, 0, 4 + v);  // Emits INC, Increases by 4 plus the number of variables

    Statement(lexLevel);   

    Mark(c + v + p);
}

// ----------------------------------------------------------------------------------------------------------------------------------------
void Program() {
    int num_Procedures = 1;
    Emit(7, 0, 10);         // Create a filler JMP, the M value is not important right now

    for(int i = 0; i < get_Lexeme_Size(); i++) {     // Loops through the entire Lex_Array
        if(lex_Array[i].tokenType == 30) {  // Checks to see if the tokentype is a Procedure
            num_Procedures++;
            Emit(7, 0, 10);     // Another filler JMP, with a random M
        }
    }
    Add_To_SymTable(3, "main", 0, 0, 0, 0, 0);  // Adds the MAIN function to the symbol Table
    procedure_Count++;

    Block(0, 0, 0); // Calls Block

    if(token != 19) {
        printf("Error: program must end with period\n");
        exit(-1);
    }

    for(int i = 0; i < num_Procedures; i++) {
        VM_Instructions[i].m = symbol_Table[Find_Procedure(i)].addr;
    }

    for(int i = 0; i < VM_Instruct_Size; i++) {
        if(VM_Instructions[i].opcode == 5) VM_Instructions[i].m = symbol_Table[Find_Procedure(i)].addr;
    }

    Emit(9, 0, 3);      // HALT
}

instruction *ParserCodegenMain(lexeme *Lex_Token_Array, int A_Directive) {
    lex_Array = Lex_Token_Array;    // Copies the Lexeme Token Array into the Lex Array
    
    token = Get_Token();    // Gets the first token

    Program();  // Calls Program

    if(A_Directive == 1) Print_General_Assembly();

    return VM_Instructions;
}