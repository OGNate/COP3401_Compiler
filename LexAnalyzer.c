#include "LexAnalyzer.h"

// Global Variable
int lexeme_Array_iterator = 0;

int get_Lexeme_Size() {
    return lexeme_Array_iterator;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// This function takes in a specific identifier and state, it will then compare and return the identifier's respective token_Type
int get_Token_Type(char *identifier, int state) {
    // State == 1 represents all special symbols. State == 2 represents all reserved words
    if(state == 1) {
        if(strncmp(identifier, "%", 1) == 0) return 1;
        else if(strncmp(identifier, "+", 1) == 0) return 4;
        else if(strncmp(identifier, "-", 1) == 0) return 5;
        else if(strncmp(identifier,  "*", 1) == 0) return 6;
        else if(strncmp(identifier,  "/", 1) == 0) return 7;
        else if(strncmp(identifier,  "=", 1) == 0) return 9;
        else if(strncmp(identifier,  "<>", 2) == 0) return 10;
        else if(strncmp(identifier,  "<=", 2) == 0) return 12;
        else if(strncmp(identifier,  "<", 1) == 0) return 11;
        else if(strncmp(identifier,  ">=", 2) == 0) return 14;
        else if(strncmp(identifier,  ">", 1) == 0) return 13;
        else if(strncmp(identifier,  "(", 1) == 0) return 15;
        else if(strncmp(identifier,  ")", 1) == 0) return 16;
        else if(strncmp(identifier,  ",", 1) == 0) return 17;
        else if(strncmp(identifier,  ";", 1) == 0) return 18;
        else if(strncmp(identifier,  ".", 1) == 0) return 19;
        else if(strncmp(identifier,  ":=", 2) == 0) return 20;
        else return -1;
    } else if(state == 2) {
        if(strcmp(identifier, "odd") == 0) return 8;
        if(strcmp(identifier, "begin") == 0) return 21;
        if(strcmp(identifier, "end") == 0) return 22;
        if(strcmp(identifier, "if") == 0) return 23;
        if(strcmp(identifier, "then") == 0) return 24;
        if(strcmp(identifier, "while") == 0) return 25;
        if(strcmp(identifier, "do") == 0) return 26;
        if(strcmp(identifier, "call") == 0) return 27;
        if(strcmp(identifier, "const") == 0) return 28;
        if(strcmp(identifier, "var") == 0) return 29;
        if(strcmp(identifier, "procedure") == 0) return 30;
        if(strcmp(identifier, "write") == 0) return 31;
        if(strcmp(identifier, "read") == 0) return 32;
        if(strcmp(identifier, "else") == 0) return 33;
        if(strcmp(identifier, "return") == 0) return 34;
        else return -1;
    } else {
        return -1;
    }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// IF the newLexeme.next_Character == #, then that means there was no special symbol behind it
// check_Ident_or_RW is a function that is used to check whether a word beginning with a letter is a reserved word or an identifier.
lexeme check_Ident_or_RW(FILE *inputFile, char **reserved_Words, int reserved_Words_Len, char character, int L_Directive) {
    lexeme newLexeme;

    // Creates a string
    char identifier[MAX_ID_LEN + 2] = "";
    strncpy(identifier, &character, 1);

    while(1 && !feof(inputFile)) {
        character = fgetc(inputFile);

        // VALID: If the read in character is an alpha or a digit and the length of the identifer string is less than the
        // MAX_ID_LEN, then the character is concatenated to the identifier string
        if((isalpha(character) != 0 || isdigit(character) != 0) && (strlen(identifier) <= MAX_ID_LEN)) {
            strncat(identifier, &character, 1);
        }

        // VALID: This if-statement checks to make sure the identifier is actually a reserved word or an identifier.
        if((isspace(character) != 0 || iscntrl(character) != 0) && (strlen(identifier) <= MAX_ID_LEN)) {

            // This for-loop checks whether the identifier is a reserved word or not, if so, the newLexeme gets its token type from
            // the get_Token_Type function. It will then print the information and then return the new Lexeme.
            for(int i = 0; i < reserved_Words_Len; i++) {
                if(strcmp(identifier, reserved_Words[i]) == 0) {
                    strcpy(newLexeme.lexeme, identifier);
                    strcpy(newLexeme.next_Character, "#");
                    newLexeme.tokenType = get_Token_Type(identifier, 2);

                    if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
                    return newLexeme;
                }
            }

            // This portion of code identifies an identifier. It prints out the information and then returns the newLexeme.
            strcpy(newLexeme.lexeme, identifier);
            strcpy(newLexeme.next_Character, "#");
            newLexeme.tokenType = 2;
            if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
            return newLexeme;
        }


        if(((isdigit(character) == 0) && (isalpha(character) == 0) && (isspace(character) == 0) && (iscntrl(character) == 0)) && (strlen(identifier) <= MAX_ID_LEN)) {
            // This for-loop checks whether the identifier is a reserved word or not, if so, the newLexeme gets its token type from
            // the get_Token_Type function. It will then print the information and then return the new Lexeme.
            for(int i = 0; i < reserved_Words_Len; i++) {
                if(strcmp(identifier, reserved_Words[i]) == 0) {
                    strcpy(newLexeme.lexeme, identifier);
                    strncpy(newLexeme.next_Character, &character, 1);
                    newLexeme.tokenType = get_Token_Type(identifier, 2);

                    if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
                    return newLexeme;
                }
            }

            // This portion of code identifies an identifier. It prints out the information and then returns the newLexeme.
            strcpy(newLexeme.lexeme, identifier);
            strncpy(newLexeme.next_Character, &character, 1);
            newLexeme.tokenType = 2;
            if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
            return newLexeme;
        }


        // INVALID: This if-statement checks to see if a valid character (alpha or digit) is present, if so but the length of the
        // identifier is greater than or equual to the MAX_ID_LEN then the identifier is considered invalid since it is longer than
        // the given max length.
        if((isalpha(character) != 0 || isdigit(character) != 0) && (strlen(identifier) > MAX_ID_LEN)) {
            printf("Error: Identifier names cannot exceed 11 characters\n");
            exit(-1);

            character = fgetc(inputFile);

            // Continues reading in characters until a non alpha or digit is reached.
            while(isalpha(character) != 0 || isdigit(character) != 0) {
                character = fgetc(inputFile);
            }

            // This if statement checks to see if the character that stopped the while loop above is a space or control character. If so, a Z is passed to the
            // newLexeme.next_Character meaning that there is no special symbol behind the invalid identifier.
            if(isspace(character) != 0 || iscntrl(character) != 0) {
                strcpy(newLexeme.lexeme, "");
                strcpy(newLexeme.next_Character, "#");
                newLexeme.tokenType = -1;
                return newLexeme;
            }

            // This if statement checks to see if the character isn't a space, control character, alpha, or digit. If this is the case,
            // the character could be a special symbol or an invalid symbol. In either case, the main function will take care of this.
            if(isspace(character) == 0 && iscntrl(character) == 0 && isalpha(character) == 0 && isdigit(character) == 0) {
                strcpy(newLexeme.lexeme, "");
                strcpy(newLexeme.next_Character, &character);
                newLexeme.tokenType = -1;
                return newLexeme;
            }
        }
    }

    // This if statement checks a potential identifier or reserve word that was not picked up due to the end of the inputFile.
    if(strcmp(identifier, "") != 0) {
        for(int i = 0; i < reserved_Words_Len; i++) {
            if(strcmp(identifier, reserved_Words[i]) == 0) {
                strcpy(newLexeme.lexeme, identifier);
                strcpy(newLexeme.next_Character, "#");
                newLexeme.tokenType = get_Token_Type(identifier, 2);

                if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
                return newLexeme;
            }
        }

        strcpy(newLexeme.lexeme, identifier);
        strcpy(newLexeme.next_Character, "#");
        newLexeme.tokenType = 2;
        if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
        return newLexeme;
    }
}

// This function prints the information of the lexemes in the lexeme array
void print_Lexeme_Array(lexeme *lexeme_Array, const int lexeme_Iterator) {
    int i = 0;

    while(i != lexeme_Iterator) {
        if(lexeme_Array[i].tokenType == 2 || lexeme_Array[i].tokenType == 3) {
            printf("%d %s ", lexeme_Array[i].tokenType, lexeme_Array[i].lexeme);
        } else {
            printf("%d ", lexeme_Array[i].tokenType);
        }
        i++;
    }
}


// IF the next_character is == #, then there is not a special symbol after the number
lexeme validNum_Check(FILE *inputFile, char digit, int L_Directive) {
    lexeme newLexeme;

    char number[MAX_NUM_LEN + 2] = "";

    strncpy(number, &digit, 1);

    while(1 && !feof(inputFile)) {
        digit = fgetc(inputFile);

        // Checks the length of number
        if(strlen(number) < MAX_NUM_LEN) {

            // Case 1: This checks to see if the read in character is a digit, if so it appends the character to number
            if(isdigit(digit) != 0) {
                strncat(number, &digit, 1);
            } else if((isdigit(digit) == 0) && (isalpha(digit) == 0)) { // THis else if statement checks characters that aren't a digit or alpha
                if(isspace(digit) != 0 || iscntrl(digit) != 0) {
                    strcpy(newLexeme.lexeme, number);
                    strcpy(newLexeme.next_Character, "#");
                    newLexeme.tokenType = 3;
                    if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
                    return newLexeme;
                } else {
                    strcpy(newLexeme.lexeme, number);
                    strncpy(newLexeme.next_Character, &digit, 1);
                    newLexeme.tokenType = 3;
                    if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
                    return newLexeme;
                }
            } else if(isalpha(digit) != 0) {    // This else if statement checks alpha characters and handles the error.
                printf("Error: Identifiers cannot begin with a digit\n");
                exit(-1);

                // Continues reading in characters until a non-alpha or non-digit is hit
                while(isalpha(digit) != 0 || isdigit(digit) != 0) digit = fgetc(inputFile);

                // Returns a newLexeme with the digit in the next_Character spot if the character is not a space or control character
                if(isspace(digit) == 0 || iscntrl(digit) != 0) {
                    strcpy(newLexeme.lexeme, "");
                    strncpy(newLexeme.next_Character, &digit, 1);
                    newLexeme.tokenType = -1;
                    return newLexeme;
                } else {
                    strcpy(newLexeme.lexeme, "");
                    strcpy(newLexeme.next_Character, "#");
                    newLexeme.tokenType = -1;
                    return newLexeme;
                }
            }
        } else { // If number is greater than or equal to the MAX_NUM_LEN and the next char is a digit or alpha, the number excess error will appear
            if(isdigit(digit) != 0 || isalpha(digit) != 0) {
                printf("Error: Numbers cannot exceed 5 digits\n");
                exit(-1);

                while(isdigit(digit) != 0 || isalpha(digit) != 0) digit = fgetc(inputFile);

                if(isspace(digit) == 0 || iscntrl(digit) != 0) {
                    strcpy(newLexeme.lexeme, "");
                    strncpy(newLexeme.next_Character, &digit, 1);
                    newLexeme.tokenType = -1;
                    return newLexeme;
                } else {
                    strcpy(newLexeme.lexeme, "");
                    strcpy(newLexeme.next_Character, "#");
                    newLexeme.tokenType = -1;
                    return newLexeme;
                }
            } else {    // If the number is greater than or equal to MAX_NUM_LEN and the next char is a space, control char, or other newLexeme with the values will return
                if(isspace(digit) == 0 || iscntrl(digit) != 0) {
                    strcpy(newLexeme.lexeme, number);
                    strncpy(newLexeme.next_Character, &digit, 1);
                    newLexeme.tokenType = 3;
                    if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
                    return newLexeme;
                } else {
                    strcpy(newLexeme.lexeme, number);
                    strcpy(newLexeme.next_Character, "#");
                    newLexeme.tokenType = 3;
                    if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
                    return newLexeme;
                }
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------
// This function checks an operator to determine whether it is valid or not
lexeme operand_Check(FILE *inputFile, char character, int L_Directive) {
    lexeme newLexeme;

    char operator[3] = "";
    strncpy(operator, &character, 1);

    character = fgetc(inputFile);

    // If there is a space, control character, digit, or letter after the initial symbol, this if statement will check to see if the symbol is valid.
    // If not, an error message will show.
    if(isspace(character) != 0 || iscntrl(character) != 0 || isdigit(character) != 0 || isalpha(character) != 0) {
        newLexeme.tokenType = get_Token_Type(operator, 1);

        if(newLexeme.tokenType == -1) {
            printf("Error: Invalid Symbol\n");
            exit(-1);

            strcpy(newLexeme.lexeme, "");
            strncpy(newLexeme.next_Character, &character, 1);
            return newLexeme;
        } else {
            strcpy(newLexeme.lexeme, operator);
            strncpy(newLexeme.next_Character, &character, 1);
            if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
            return newLexeme;
        }
    } else {    // If there is a potential second character to the special symbol

        // Checks to see if there is a closing parenthesis ")" 
        if(get_Token_Type(operator, 1) == 16 && get_Token_Type(&character, 1) == 18) {
            newLexeme.tokenType = get_Token_Type(operator, 1);
            strcpy(newLexeme.lexeme, operator);
            strcpy(newLexeme.next_Character, &character);
            if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
            return newLexeme;
        } 

        strncat(operator, &character, 1);

        if(strcmp(operator, "/*") == 0) {
            while(1) {
                character = fgetc(inputFile);

                if(character == '*') {
                    character = fgetc(inputFile);
                    if(character == '/') break;
                }
            }
            strcpy(newLexeme.lexeme, "");
            strcpy(newLexeme.next_Character, "#");
            newLexeme.tokenType = -1;
            return newLexeme;
        }

        if(get_Token_Type(operator, 1) == 20 || get_Token_Type(operator, 1) == 12 || get_Token_Type(operator, 1) == 14 || get_Token_Type(operator, 1) == 10) {
            newLexeme.tokenType = get_Token_Type(operator, 1);
            strcpy(newLexeme.lexeme, operator);
            strcpy(newLexeme.next_Character, "#");
            if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
            return newLexeme;
        }
        
        newLexeme.tokenType = get_Token_Type(operator, 1);
        strcpy(newLexeme.lexeme, operator);
        strcpy(newLexeme.next_Character, "#");

        if(L_Directive == 1) printf("\t%s   %d\n", newLexeme.lexeme, newLexeme.tokenType);
        return newLexeme;
    }
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Main function reads through an entire inputfile and scans in all characters and tokenizes them
lexeme* LexMain(int argc, char *argv[], int L_Directive) {

    // Creates an io stream to the inputfile
    FILE *inputFile = fopen(argv[1], "r");

    // Creates a constant array of strings that are both reserved words and special symbols
    char *reserved_Words[] = {"const", "var", "procedure", "call", "if", "then", "else", "while", "do", "read", "write", "odd", "return", "begin", "end"};
    char *special_Symbols[] = {"+", "-", "*", "/", "(", ")", "=", ",", ".", "<", ">", ";", ":", "%"};

    lexeme *lexeme_Array = malloc(500 * sizeof(lexeme));
    lexeme newLexeme;

    char character;

    if(L_Directive == 1) {
        printf("Lexeme Table:\n");
        printf("\tlexeme   token type\n");
    }

    character = fgetc(inputFile);

    while(!feof(inputFile)) {

        // Checks the first value of a potential identifer to see if its an alpha, if so, it will analyze the following characters to see
        // if it is a valid identifier or reserved word.
        if(isalpha(character) != 0) {
            newLexeme = check_Ident_or_RW(inputFile, reserved_Words, sizeof(reserved_Words) / sizeof(reserved_Words[0]), character, L_Directive);

            // This checks to see if there is a potential special symbol right after an identifier. Z == no special symbol. If there's no Z,
            // character is assigned the value of the symbol right after the identifer while then continueing to the next iteration of the while loop.
            // In either case of Z or not Z, the newLexeme is checked to see if its tokeyType is greater than or equal to 1. If so, then it was a valid
            // identifier or reserved word and it is added to the lexeme Array.
            if(strcmp(newLexeme.next_Character, "#") != 0) {
                character = newLexeme.next_Character[0];

                if(newLexeme.tokenType >= 1) {
                    lexeme_Array[lexeme_Array_iterator] = newLexeme;
                    lexeme_Array_iterator++;
                }

                continue;
            }

            if(newLexeme.tokenType >= 1) {
                lexeme_Array[lexeme_Array_iterator] = newLexeme;
                lexeme_Array_iterator++;
            }
        }

        // Checks the see if a character is a digit, if so, it will determine if the following digits will make a valid number or not.
        // If it is not a valid number, an error message will appear
        if(isdigit(character) != 0) {
            newLexeme = validNum_Check(inputFile, character, L_Directive);

            if(strcmp(newLexeme.next_Character, "#") != 0) {
                character = newLexeme.next_Character[0];

                if(newLexeme.tokenType >= 1) {
                    lexeme_Array[lexeme_Array_iterator] = newLexeme;
                    lexeme_Array_iterator++;
                }

                continue;
            }

            if(newLexeme.tokenType >= 1) {
                lexeme_Array[lexeme_Array_iterator] = newLexeme;
                lexeme_Array_iterator++;
            }
        }

        // This if statement checks to see if operators are valid. If so it will tokenize them. If not an error message will appear.
        if(isdigit(character) == 0 && isalpha(character) == 0 && isspace(character) == 0 && iscntrl(character) == 0) {

            int is_Symbol = get_Token_Type(&character, 1);

            // Checks if the given symbol is apart of the symbol set. It will also check for ":" since it is the only multiple
            // character symbol that does not have a valid first digit in the symbol set.
            if(is_Symbol == - 1 && character != ':') {
                printf("Error: Invalid Symbol\n");
                exit(-1);
                character = fgetc(inputFile);
                continue;
            } else { // Checks the rest of the symbols
                for(int i = 0; i < sizeof(special_Symbols) / sizeof(special_Symbols[0]); i++) {
                    if((strncmp(special_Symbols[i], &character, 1) == 0) || character == ':') {
                        newLexeme = operand_Check(inputFile, character, L_Directive);

                        if(strcmp(newLexeme.next_Character, "#") != 0) {
                            character = newLexeme.next_Character[0]; 

                            if(newLexeme.tokenType >= 1) {
                                lexeme_Array[lexeme_Array_iterator] = newLexeme;
                                lexeme_Array_iterator++;
                            }
                            break;
                        }

                        if(newLexeme.tokenType >= 1) {
                            lexeme_Array[lexeme_Array_iterator] = newLexeme;
                            lexeme_Array_iterator++;
                        }

                        character = fgetc(inputFile);
                    }
                }
                continue;
            }
        }
        character = fgetc(inputFile);
    }


    if(L_Directive == 1) {
        // Prints the lexeme array
        printf("\nToken List:\n");
        print_Lexeme_Array(lexeme_Array, lexeme_Array_iterator);
        printf("\n\n\n");
    }

    // Closes the inputFile Stream
    fclose(inputFile);
    return lexeme_Array;
}