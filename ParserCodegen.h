#ifndef PCG_H
#define PCG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "CompilerMain.h"
#include "LexAnalyzer.h"

#define MAX_SYMBOL_TABLE_SIZE 500

typedef struct symbol {
    int kind;       // Const = 1, Var = 2
    char name[12];  // Name up to 12 characters long
    int val;        // Number
    int level;      // Lexigraphical Level
    int addr;       // M Address
} symbol;

int symbol_Table_Check(const symbol symbol_Table[], const int *symbol_Table_Size, const char *search_Name);
void add_To_SymTable(symbol symbol_Table[], int *symbol_Table_Size, int kind, char *name, int number, int level, int address);
int get_Token(lexeme *lexeme_Array, int *lexeme_Array_Size);
void constant_Declaration(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token);
void block(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token);
void Program(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size);
void ParserCodegenMain(lexeme *Lex_Token_Array);


#endif
