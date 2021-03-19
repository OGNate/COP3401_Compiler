#include "ParserCodegen.h"

// GLOBAL VARIABLES
instruction VM_Instructions[MAX_SYMBOL_TABLE_SIZE];    // Global variable that can be modified from anywhere in the program
int VM_Instructions_Size = 0;   // Keeps track of the size of the VM

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// This function retrieves the cooresponding 3-letter Operator name for the given OP_Code
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

// -------------------------------------------------------------------------------------------------------------------------------------------
// This function prints General Assembly of the compiler
void print_General_Assembly() {
    printf("Generated Assembly:\n");
    printf("Line\tOP\tL\tM\n");
    
    for(int i = 0; i < VM_Instructions_Size; i++) {
        printf("%d\t%s\t%d\t%d\n", i, get_OP_Name(VM_Instructions[i].opcode), VM_Instructions[i].l, VM_Instructions[i].m);
    }
    printf("\n\n");
}

// ----------------------------------------------------------------------------------------------------------------------------------------
// Checks the symbol table to see if the searched for name is found. If so, it will return the index, if not, it will
// return -1.
int symbol_Table_Check(const symbol symbol_Table[], const int *symbol_Table_Size, const char *search_Name) { 
    for(int i = 0; i < (*symbol_Table_Size); i++) {
        if(strcmp(symbol_Table[i].name, search_Name) == 0) return i;
    }
    return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------
// Adds a constant to the symbol table
void add_To_SymTable(symbol symbol_Table[], int *symbol_Table_Size, int kind, char *name, int number, int level, int address) {
    symbol newSymbol;   // Declares a new symbol for the symbol table

    newSymbol.kind = kind;
    strcpy(newSymbol.name, name);
    newSymbol.val = number;
    newSymbol.level = level;
    newSymbol.addr = address;

    symbol_Table[*symbol_Table_Size] = newSymbol;   // Adds the new symbol to the symbol table
    (*symbol_Table_Size)++;     // Increments the symbol table size
}

// -----------------------------------------------------------------------------------------------------------------------------------------
// Retrieves the next token in the lexeme Array. It will then increment the Lexeme_Array size to make sure it will be in the right place
// next time get_Token is called.
int get_Token(lexeme *lexeme_Array, int *lexeme_Array_Size) {
    int token = lexeme_Array[*lexeme_Array_Size].tokenType;
    (*lexeme_Array_Size)++;
    return token;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------
// This function emits a new instruction into the VM instructions which will eventually be passed to the Virtual Machine.
void emit(int Op_Code, int L, int M) {
    instruction new_Instruction;        // A new instruction is declared and initialized with the given values
    new_Instruction.opcode = Op_Code;
    new_Instruction.l = L;
    new_Instruction.m = M;

    VM_Instructions[VM_Instructions_Size] = new_Instruction;    // The new instruction is added the the array of Virtual Machine instructions
    VM_Instructions_Size++;                                     // The virtual machine instruction size is incremented by one
}

// -----------------------------------------------------------------------------------------------------------------------------------------
// This function will Check whether there are any constants declared in the block. If so, it will check the validity as well. It will then
// add the constants to the symbol table if no errors occur.
void constant_Declaration(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token) {
    (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Retrieves the next token 
    
    if((*token) == 28) {    // First checks if the read in token is a constant
        do {    
            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            if((*token) != 2) { // Checks to make sure that the token after const is an identifier. If not, an error will occur.
                printf("const, var, and read keywords must be followed by identifier\n");
                exit(-1);
            }

            // This if-statement checks to see if the identifier name is in the symbol table. If the name is in the symbol
            // table, it will cause an error since the name has already been declared.
            if(symbol_Table_Check(symbol_Table, symbol_Table_Size, lexeme_Array[(*lexeme_Array_Size) - 1].lexeme) != -1) {
                printf("symbol name has already been declared\n");
                exit(-1);
            }

            int ident_Name_Index = (*lexeme_Array_Size) - 1;    // Because the identifier is valid, we will save the index so we can insert in the symbol table

            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            if((*token) != 9) { // Checks if the the next token after the identifier name is "=", if not an error will occur.
                printf("constants must be assigned with =\n");
                exit(-1);
            }

            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            if((*token) != 3) { // Checks if the next token after the "=" is a numeric value. If not an error will occur.
                printf("constants must be assigned an integer value\n");
                exit(-1);
            }

            int ident_Value_Index = (*lexeme_Array_Size) - 1;   // Since the token was a valid number, it will save the value of the index
            int integer_Value = atoi(lexeme_Array[ident_Value_Index].lexeme);   // Converts the numeric string value to an integer value

            // Adds the constant to the symbol table
            add_To_SymTable(symbol_Table, symbol_Table_Size, 1, lexeme_Array[ident_Name_Index].lexeme, integer_Value, 0, 0);  

            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token            

        } while((*token) == 17);    // This do-while loop will run while the token is a ",". It will always run at least once.

        // Checks to see if there is a semicolon (;) after all constant declarations. If not, an error will occur
        if((*token) != 18) {
            printf("constant and variable declarations must be followed by a semicolon\n");
            exit(-1);
        }

        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
    }
}

// ------------------------------------------------------------------------------------------------------------------------------------------
// This function take care of any variable declarations done within the block. If no errors occur with identifiers, symbol names, or missing
// semicolons; the function will add the variables to the symbol table. The number of variables is returned.
int variable_Declaration(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token) {
    int num_Variables = 0; 

    if((*token) == 29) {
        do {
            num_Variables++;    // Increment the number of variables
            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            // Checks to see if the next token is an identifier. If not, an error occurs and the program stops.
            if((*token) != 2) {
                printf("const, var, and read keywords must be followed by identifier\n");
                exit(-1);
            }

            // Checks to see if the given token name is present in the symbol table, if so, an error occurs.
            if(symbol_Table_Check(symbol_Table, symbol_Table_Size, lexeme_Array[(*lexeme_Array_Size) - 1].lexeme) != -1) {
                printf("symbol name has already been declared\n");
                exit(-1);
            }

            int ident_Name_Index = (*lexeme_Array_Size) - 1;    // Gets the index of the identifier

            // Adds the variable to the symbol table
            add_To_SymTable(symbol_Table, symbol_Table_Size, 2, lexeme_Array[ident_Name_Index].lexeme, 0, 0, num_Variables + 3);

            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

        } while((*token) == 17);    // While a comma is read in after a variable, this do-while loop will continue.

        // If there is not a semicolon after the variable declarations, an error will occur.
        if((*token) != 18) {
            printf("constant and variable declarations must be followed by a semicolon\n");
            exit(-1);
        }

        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
    }
    return num_Variables;
}

// ---------------------------------------------------------------------------------------------------------------------------------------
// This function checks the factor and whether its valid or not
void factor(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token)
{
    int symIdx;

    if((*token) == 2)
    {
        symIdx = symbol_Table_Check(symbol_Table, symbol_Table_Size, lexeme_Array[(*lexeme_Array_Size) - 1].lexeme);

        if(symIdx == -1)
            printf("symbol has already been declared\n");

        if(symbol_Table[symIdx].kind == 1)
            emit(1, 0, symbol_Table[symIdx].val);   // Adds the LIT instruction to the Virtual Machine Instruction Array

        else
            emit(3, 0, symbol_Table[symIdx].addr);  // Adds the LOD instruction to the Virtual Machine Instruction Array

        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
    }

    else if((*token) == 3)
    {
        int value = atoi(lexeme_Array[(*lexeme_Array_Size) - 1].lexeme);    // Converts the string numeric value to an integer value
        emit(1, 0, value);      // Adds the LIT instruction to the Virtual Machine Instruciton Array
        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
    }

    else if((*token) == 15)
    {
        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
        expression(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);

        if((*token) != 16)
            printf("need to close parenthesis\n");

        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
    }

    else
      printf("error\n");
}

// ------------------------------------------------------------------------------------------------------------------------------------------
// This function checks the terms and whether they are valid or not
void term(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token)
{
    factor(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);
    while((*token) == 6 || (*token) == 7 || (*token) == 1)
    {
        if((*token) == 6)
        {
            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
            factor(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);
            emit(2, 0, 4); // Adds the multiplication (MUL) instruction to the Viritual Machine Instruction Array
        }

        else if((*token) == 7)
        {
            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
            factor(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);
            emit(2, 0, 5);  // Adds the division (DIV) instruction to the Virtual Machine Instruction Array
        }

        else
        {
            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
            factor(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);
            emit(2, 0, 7);  // Adds the modulus (MOD) instruction to the Virtual Machine Instruction Array
        }
    }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// This function checks the expression and makes sure its valid. If valid, the statement is added to teh Virtual Machine Instructions
void expression(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token)
{
    if((*token) == 5)
    {
        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
        term(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);
        emit(2, 0, 1);  // Adds the Negative (NEG) instruction to the Virtual Machine Instruction Array

        while((*token) == 4 || (*token == 5))
        {
            if((*token) == 4)
            {
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                term(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);
                emit(2, 0, 2);  // Adds the Add (ADD) instruction to the Virtual Machine Instruction Array
            }

            else
            {
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                term(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);
                emit(2, 0, 3);  // Adds the Sub (SUB) instruction to the Virtual Machine Instruction Array
            }
        }
    }

    else
    {
        if((*token) == 4)
          (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

        term(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);

        while((*token) == 4 || (*token) == 5)
        {
            if((*token) == 4)
            {
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                term(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);
                emit(2, 0, 2);  // Adds the Add (ADD) instruction to the Virtual Machine Instruction Array
            }

            else
            {
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                term(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);
                emit(2, 0, 3);  // Adds the Sub (SUB) instruction to the Virtual Machine Instruction Array
            }
        }
    }
}

// -----------------------------------------------------------------------------------------------------------------------------------------
// This function checks for statements that begin with keywords in the PL/0 grammar
void statement(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token) {
    int symbol_Index, jpc_Index;

    // If the token is an identifier
    switch(*token) {
        // Case 2: Checks to see if the statement starts with an identifier
        case 2:
            // Checks to see if the identifier is in the symbol table
            symbol_Index = symbol_Table_Check(symbol_Table, symbol_Table_Size, lexeme_Array[(*lexeme_Array_Size) - 1].lexeme);  

            if(symbol_Index == -1) {    // If the identifier is not found in the symbol table (-1), an error will occur and the program will exit
                printf("undeclared symbol\n");
                exit(-1);
            }

            if(symbol_Table[symbol_Index].kind != 2) {  // If the identifier is not a variable, an error will occur since only variables can be changed
                printf("only variable values may be altered\n");
                exit(-1);
            }

            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            if((*token) != 20) {    // If the next token after the identifier is not a ":=" symbol, an error will occur and the program will exit
                printf("assignment statements must use :=\n");
                exit(-1);
            }

            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            expression(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);    // Checks the expression following the ":=" operator

            emit(4, 0, symbol_Table[symbol_Index].addr);    // Adds the STORE (STO) instruction to the Virtual Machine Instructions array
            break;        // Breaks out of the switch statement

        // Case 21: Checks to see if the statement starts with the word "begin"
        case 21:
            do {
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

                statement(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token); // Recursive call to STATEMENT 
            } while((*token) == 18);    // Continues to loop while there are statements within the "begin" and "end" keywords

            if((*token) != 22) {    // Checks to make sure there is eventually an "end" keyword, if not an error occurs and the program will exit
                printf("begin must be followed by end\n");
                exit(-1);
            }

            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
            break;

        // Case 23: Checks to see if the statement starts with the word "if"
        case 23: 
            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            condition(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token); // Checks to see what the condition is in the expression

            jpc_Index = VM_Instructions_Size - 1;   // Gets the current Top index of the Virtual Machine Instructions array

            emit(8, 0, jpc_Index);  // Adds the JPC instruction to the Virtual Machine Instructions Array

            if((*token) != 24) {    // Checks to make sure there is a "then" keyword eventually after the "if" keyword, if not an error will occur
                printf("if must be followed by then\n");
                exit(-1);
            }

            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            statement(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token); // Recursively calls the statement function to check for any other statements

            VM_Instructions[jpc_Index].m = VM_Instructions_Size - 1;    // Jumps to the current top of the Virtual Machine Instructions Array
            break;

        // Case 25: Checks to see if the statement starts with the keyword "while"
        case 25:
            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            int loop_Index = VM_Instructions_Size - 1; // Gets the current top index of the Virtual Machine Instructions Array

            condition(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token); // Checks the conditions after the "while" keyword

            if((*token) != 26) {    // Checks to make sure that there is a "do" keyword after the "while" keyword, if not an error occurs
                printf("while must be followed by do\n");
                exit(-1);
            }

            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            jpc_Index = VM_Instructions_Size - 1;   // Gets the current top index of the VM Instructions Array

            emit(8, 0, jpc_Index);  // Adds the JPC instruction to the Virtual Machine Instructions Array

            statement(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);

            emit(7, 0, loop_Index); // Adds the JMP instruction to the Virtual Machine Instructions Array

            VM_Instructions[jpc_Index].m = VM_Instructions_Size - 1;    // Jumps to the current top of the Virtual Mahine Instructions Array
            break;

        // Case 32: Checks to see if the statement starts with the keyword "read"
        case 32:
            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            if((*token) != 2) { // Checks to see if the next token is an identifier
                printf("const, var, and read keywords must be followed by identifier\n");
                exit(-1);
            }

            symbol_Index = symbol_Table_Check(symbol_Table, symbol_Table_Size, lexeme_Array[(*lexeme_Array_Size) - 1].lexeme);  // Gets the symbol index of the identifier in the symbol table

            if(symbol_Index == -1) {    // Checks to see if the identifier is in the symbol table, if it is -1, it is not and an error occurs
                printf("undeclared symbol\n");
                exit(-1);
            }

            if(symbol_Table[symbol_Index].kind != 2) {  // Checks to make sure the identifier is a variable
                printf("Only variable values may be altered\n");
                exit(-1);
            }

            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            emit(9, 0, 2);   // Adds the READ instruction to the Virtual Machine Instructions Array
            emit(4, 0, symbol_Table[symbol_Index].addr);    // Adds the store (STO) instruction to the Virtual Machine Instructions Array
            break;

        // Case 31: Checks to see if the statement starts with the keyword "write"
        case 31:
            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

            expression(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);    // Checks for expressions after the write statement

            emit(9, 0, 1);  // Adds the WRITE instruction to the Virtual Machine Insructions Array
            break;

        default:    // Default case that just returns
            break;
    }
}
// --------------------------------------------------------------------------------------------------------------------------------------------------------
// This function checks for conditional operators
void condition(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token) {
    if((*token) == 8) { // Checks to see if the condition is "odd" (Essentially the "!" or "not" operator in C and python)
        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

        expression(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);    // Calls the expression function to find out the expression after the condition

        emit(2, 0, 6);  // Adds the ODD instruction to the Virtual Machine Instructions Array
    } else {
        expression(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);    // Calls the expression function to find out the expression after the condition

        // Checks the values of the token to see if it is any of the other conditional symbols
        switch(*token) {
            // Case 9: Handles the "=" equals symbol
            case 9:
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token 
                expression(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);    // Calls the expression function to find out the expression after the condition
                emit(2, 0, 8);      // Adds the Equals symbol (EQL) instruction to the Virtual Machine Instructions Array
                break;
            
            // Case 10: Handles the "<>" not equals symbol
            case 10:
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                expression(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);    // Calls the expression function to find out the expression after the condition
                emit(2, 0, 9);      // Adds the Not equal symbol (NEQ) instruction to the Virtual Machine Instructions Array
                break;

            // Case 11: Handles the "<" less than symbol
            case 11:
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                expression(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);    // Calls the expression function to find out the expression after the condition
                emit(2, 0, 10);      // Adds the Less symbol (LSS) instruction to the Virtual Machine Instructions Array
                break;    

            // Case 12: Handles the "<=" less than or equal to symbol
            case 12:
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                expression(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);    // Calls the expression function to find out the expression after the condition
                emit(2, 0, 11);      // Adds the Less than or equal to symbol (LEQ) instruction to the Virtual Machine Instructions Array
                break;

            // Case 13: Handles the ">" greater than symbol
            case 13:
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                expression(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);    // Calls the expression function to find out the expression after the condition
                emit(2, 0, 12);      // Adds the Greater than symbol (GTR) instruction to the Virtual Machine Instructions Array
                break;

            // Case 11: Handles the ">=" greater than or equal to symbol
            case 14:
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                expression(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);    // Calls the expression function to find out the expression after the condition
                emit(2, 0, 13);      // Adds the Greater than or equal to (GEQ) instruction to the Virtual Machine Instructions Array
                break;

            // Default: If there are no relational operators, an error will occur and the program will exit
            default:
                printf("condition must contain comparison operator\n");
                exit(-1);
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------------------------
// This block function handles the a block of the program. It will read in any constant declarations first, variable declarations second,
// and handle any statements that come after it. It will also create space to eventually generate code for the variables and constants.
void block(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token) {
    // Checks to see if there are any constants in the block. If so, with no errors, they will be added to the symbol Table.
    constant_Declaration(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);  

    // Checks to see if there are any variables in the block declaration
    int num_Vars = variable_Declaration(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);

    emit(6, 0, num_Vars + 4);   // Adds the increment (INC) instruction to the Virtual Machine Instructions Array
    
    statement(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token); // Calls the statement function
}


// ----------------------------------------------------------------------------------------------------------------------------------------
void Program(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size) {
    int token;
    block(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);

    if(token != 19) {
        printf("program must end with period\n");
        exit(-1);
    }

    emit(9, 0, 3); // Adds the HALT instruction to the Virtual Machine Instruction Array
}

// ------------------------------------------------------------------------------------------------------------------------------------------
// MAIN function that runs through the entire parsing and code generation process
instruction *ParserCodegenMain(lexeme *Lex_Token_Array, int A_Directive) {
    symbol symbol_Table[MAX_SYMBOL_TABLE_SIZE]; // Creates an array of type symbol with a size of MAX_SYMBOL_TABLE_SIZE
    int symbol_Table_Size = 0;

    lexeme *lexeme_Array = Lex_Token_Array; // lexeme_Array is now a copy of the original lexeme token array
    int lexeme_Array_Size = 0;

    // Starts the parsing and code generation process
    Program(lexeme_Array, &lexeme_Array_Size, symbol_Table, &symbol_Table_Size);

    // Prints the General Assembly only if the A_Directive is given
    if(A_Directive == 1) print_General_Assembly();

    return VM_Instructions;
}