#ifndef VM_H
#define VM_H

typedef struct instruction{
    int opcode;
    char op[4];
    int l;
    int m;
} instruction;

int base(int stack[], int level, int BP);
int main(int argc, char *argv[]);

#endif