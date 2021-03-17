#include "ParserCodegen.h"

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
// ------------------------------------------------------------------------------------------------------------------------------------------

void expression(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token)
{
    if((*token) == 5)
    {
        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
        term(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);
        //emit NEG

        while((*token) == 4 || (*token == 5))
        {
            if((*token) == 4)
            {
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                term(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);
                //emit ADD
            }

            else
            {
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                term(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);
                //emit SUB
            }
        }
    }

    else
    {
        if((*token) == 4)
          (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token

        term(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);

        while((*token) == 4 || (*token) == 5)
        {
            if((*token) == 4)
            {
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                term(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);
                //emit ADD
            }

            else
            {
                (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
                term(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);
                //emit SUB
            }
        }
    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------

void term(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token)
{
    factor(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);
    while((*token) == 6 || (*token) == 7 || (*token) == 1)
    {
        if((*token) == 6)
        {
            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
            factor(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);
            //emit MUL
        }

        else if((*token) == 7)
        {
            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
            factor(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);
            //emit DIV
        }

        else
        {
            (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
            factor(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);
            //emit MOD
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------------------------

void factor(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token)
{
    int symIdx;

    if((*token) == 2)
    {
        symIdx = symbol_Table_Check(symbol_Table, symbol_Table_Size, lexeme_Array[(*lexeme_Array_Size) - 1].lexeme);

        if(symIdx == -1)
            printf("symbol has already been declared\n");

        if(symbol_Table[symIdx].kind == 1)
            //emit LIT
            printf("garbage so else works for now");

        else
            //emit LOD

        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
    }

    else if((*token) == 3)
    {
        //emit LIT
        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
    }

    else if((*token) == 15)
    {
        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
        expression(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);

        if((*token) != 16)
            printf("need to close parenthesis\n");

        (*token) = get_Token(lexeme_Array, lexeme_Array_Size);  // Reading in a new token
    }

    else
      printf("error\n");
}

// ---------------------------------------------------------------------------------------------------------------------------------------
// This block function handles the a block of the program. It will read in any constant declarations first, variable declarations second,
// and handle any statements that come after it. It will also create space to eventually generate code for the variables and constants.
void block(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token) {
    // Checks to see if there are any constants in the block. If so, with no errors, they will be added to the symbol Table.
    constant_Declaration(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);
    int num_Vars = variable_Declaration(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, token);
    // emit INC(M = 4 + num_Vars)
    // STATEMENT()
}


// ----------------------------------------------------------------------------------------------------------------------------------------
void Program(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size) {
    int token;
    block(lexeme_Array, lexeme_Array_Size, symbol_Table, symbol_Table_Size, &token);

    if(token != 19) {
        printf("program must end with period\n");
        exit(-1);
    }

    // EMIT HALT
}

// ------------------------------------------------------------------------------------------------------------------------------------------
void ParserCodegenMain(lexeme *Lex_Token_Array) {
    symbol symbol_Table[MAX_SYMBOL_TABLE_SIZE]; // Creates an array of type symbol with a size of MAX_SYMBOL_TABLE_SIZE
    int symbol_Table_Size = 0;

    lexeme *lexeme_Array = Lex_Token_Array; // lexeme_Array is now a copy of the original lexeme token array
    int lexeme_Array_Size = 0;

    // Starts the parsing process
    Program(lexeme_Array, &lexeme_Array_Size, symbol_Table, &symbol_Table_Size);


    for(int i = 0; i < symbol_Table_Size; i++) {
        printf("kind: %d   name: %s   value: %d   level: %d   address: %d\n", symbol_Table[i].kind, symbol_Table[i].name, symbol_Table[i].val, symbol_Table[i].level, symbol_Table[i].addr);
    }

}
