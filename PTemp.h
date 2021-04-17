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

// Helper Functions
void Emit(int OP_Code, int L, int M);
void Add_To_SymTable(int kind, char *name, int number, int level, int address, int mark, int param);
int Get_Token();
int Find_Procedure(int prodecureIndex);
void Mark(int count);
int Symbol_Table_Check(char *checkName, int checkLevel);
int Symbol_Table_Search(char *name, int lex_Level, int kind);
char* get_OP_Name(int OP_Code);
void Print_General_Assembly();

// Main Code Segments
int Const_Declaration(int lex_Level);
int Variable_Declaration(int lex_Level, int param);
int Procedure_Declaration(int lex_Level);
void Statement(int lex_Level);
void Condition(int lex_Level);
void Expression(int lex_Level);
void Term(int lex_Level);
void Factor(int lex_Level);
void Block(int lexLevel, int param, int prodedureIndex);
void Program();
instruction *ParserCodegenMain(lexeme *Lex_Token_Array, int A_Directive);

#endif