#ifndef VM_H
#define VM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 50
// 100
#define MAX_STACK_HEIGHT 5000
#define MAX_CODE_LENGTH 5000

typedef struct instruction{
    int opcode;
    char op[4];
    int l;
    int m;
} instruction;

int base(int stack[], int level, int BP);
void VM_Main(instruction *VM_Instructions, int V_Directive);

#endif