#ifndef PCG_H
#define PCG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "CompilerMain.h"
#include "LexAnalyzer.h"
#include "VM.h"

#define MAX_SYMBOL_TABLE_SIZE 5000

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
void emit(int Op_Code, int L, int M);
void constant_Declaration(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token);
int variable_Declaration(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token);
void factor(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token);
void term(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token);
void expression(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token);
void statement(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token);
void condition(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token);
void block(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size, int *token);
void Program(lexeme *lexeme_Array, int *lexeme_Array_Size, symbol symbol_Table[], int *symbol_Table_Size);
instruction *ParserCodegenMain(lexeme *Lex_Token_Array);


#endif
