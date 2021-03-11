#ifndef LA_H
#define LA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ID_LEN 11
#define MAX_NUM_LEN 5

typedef struct lexeme{
    int tokenType;
    char lexeme[MAX_ID_LEN + 1];
    char next_Character[2];
}lexeme;  

int get_Token_Type(char *identifier, int state);
lexeme check_Ident_or_RW(FILE *inputFile, char **reserved_Words, int reserved_Words_Len, char character);
void comment_Check(FILE *inputFile);
void print_Lexeme_Array(lexeme *lexeme_Array, const int lexeme_Iterator);
lexeme validNum_Check(FILE *inputFile, char digit);
lexeme operand_Check(FILE *inputFile, char character);
lexeme* LexMain(int argc, char *argv[]);

#endif