#ifndef PTEMP_H
#define PTEMP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "CompilerMain.h"
#include "LexAnalyzer.h"
#include "VM.h"

#define MAX_SYMBOL_TABLE_SIZE 500

typedef struct symbol {
    int kind;       // Const = 1 , Var = 2, Procedure = 3
    char name[10];
    int val;        // Number
    int level;      // Lexigraphical Level
    int addr;       // Address
    int mark;       // Used to indicate that code has been generated already for a block    
    int param;      // Used to indicate if the parameter for a procedure has been declared
} symbol;


void Emit(int OP_Code, int L, int M);
void Add_To_SymTable(int kind, char *name, int number, int level, int address, int mark, int param);
int Get_Token();
void Program();
void ParserCodegenMain(lexeme *Lex_Token_Array, int A_Directive);

#endif