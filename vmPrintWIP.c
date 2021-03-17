/*
    Nathan Wilson and Dillon Garcia
    HW1 P-Machines
    COP3402 -> Systems Software -> Spring 2021
    Professor Montagne
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_HEIGHT 50
#define MAX_CODE_LENGTH 100

typedef struct instruction{
    int opcode;
    int l;
    int m;
} instruction;

typedef struct stackStore{
    int BP;
    int SP;
    int PC;
    char op[3];
    int inc;          //if an increment occurs, inc = 1
} stackStore;

// Base function used to help find a variable in a different activation record some L levels down.
int base(int stack[], int level, int BP) {
    int base = BP;
    int counter = level;

    while(counter > 0) {
        base = stack[base];
        counter--;
    }
    return base;
}

void printVM(stackStore *stackStore, instruction *instruction, int arrayIterator, int stack[])
{
    int i = 0;

    printf("\t\tPC   BP   SP   stack\n");
    printf("Initial values: %d    %d    %d\n", stackStore[i].PC, stackStore[i].BP, stackStore[i].SP);

    for(i = 1; i <= arrayIterator; i++)
    {

        printf("%d  %s", stackStore[i].PC, stackStore[i].op);
        printf("  %d  %d   %d   %d    %d    ", instruction[i].l, instruction[i].m, stackStore[i].PC, stackStore[i].BP, stackStore[i].SP);

        if(stackStore[i].inc == 1)
        {
            for(int i = 0; i <= stackStore[i].SP; i++)
            {
                printf("%d ", stack[i]);
            }
        }

        printf("\n");
    }

    printf("\n");
}

int main(int argc, char *argv[]) {
    instruction IR;

    // The name of the input file is given through command line arguments
    FILE * inputFile = fopen("test1.txt", "r");

    // Creates an array called text of type Instruction_Register with a max length of MAX_CODE_LENGTH
    instruction text[MAX_CODE_LENGTH];

    // Creates our stack array called "stack" of type int witha  max length of MAX_STACK_HEIGHT
    int stack[MAX_STACK_HEIGHT];

    // An array that holds the index of the SP when the CAL of a function is made. The array and the height are used
    // solely for the purpose of printing "| " in the proper place in the output.
    int AR_Seperator[MAX_STACK_HEIGHT];
    int AR_Sep_Height = 0;

    // Initializes all values within stack to 0
    for(int i = 0; i < MAX_STACK_HEIGHT; i++)
    {
      stack[i] = 0;
    }
    // Incremental value used to get all values from the input file
    int textIncrementor = 0;

    // Will run until the end of the inputFile. This while loop will take in each value from each line
    // and assign each newInstruction to the text array.

    instruction *instructionData = malloc(sizeof(instruction) * 100);

    stackStore *stackStore = malloc(sizeof(stackStore) * 100);
    int arrayIterator = 1;

    while(!feof(inputFile)) {
        fscanf(inputFile, "%d %d %d", &text[textIncrementor].opcode, &text[textIncrementor].l, &text[textIncrementor].m);
        textIncrementor++;
    }

    // Closes the file stream
    fclose(inputFile);

    // Setting up Initial Values
    stackStore[0].SP = -1;
    stackStore[0].BP = 0;
    stackStore[0].PC = 0;

    int SP = 0;
    int BP = 0;
    int PC = 0;

    int count = 0;
    int input;

    // Halt flag when set to 0 will end the program
    int halt = 1;

    // Printing the initial values of the program variables
    printf("\t\tPC   BP   SP   stack\n");
    printf("Initial values: %d    %d    %d\n", PC, BP, SP);


    while(halt != 0) {
        // FETCH CYCLE
        IR.opcode = text[PC].opcode;
        IR.l = text[PC].l;
        IR.m = text[PC].m;

        // Program Counter (PC) is incremented by 1 to point to the next instruction
        PC += 1;

        // EXECUTE CYCLE:
        // This switch case will read the Operation from the Instruction Register which is used to determine
        // which operation to perform.
        switch(IR.opcode) {

            // LIT: Increments the Stack Pointer (SP) by one and inserts the given value (IR->M) onto the top of the stack.
            case 1:
                // Assigns the opcode to its operation
                //printf("%d  %s", PC - 1, "LIT");
                stackStore[arrayIterator].PC = PC - 1;
                strcpy(stackStore[arrayIterator].op, "LIT");

                SP += 1;
                stack[SP] = IR.m;

                break;

            // OPR: Performs some type of operation
            case 2:
                //printf("%d  ", PC - 1);
                stackStore[arrayIterator].PC = PC - 1;

                // Checks to make sure that IR.m is within the given bounds
                if((IR.m >= 0) && (IR.m <= 13)) {

                    strcpy(stackStore[arrayIterator].op, "OPR");
                    switch(IR.m) {

                        // RTN: Returns one activation register down.
                        case 0:
                            stack[BP - 1] = stack[SP];
                            SP = BP - 1;
                            BP = stack[SP + 2];
                            PC = stack[SP + 3];
                            //printf("%s", "RTN");

                            // Reduces the height of the AR_Seperator array
                            AR_Sep_Height--;

                            break;

                        // NEG: returns the negative number of what is in stack[SP]
                        case 1:
                            stack[SP] = -1 * stack[SP];
                            //printf("%s", "NEG");
                            break;

                        // ADD: Adds two numbers
                        case 2:
                            SP -= 1;
                            stack[SP] = stack[SP] + stack[SP + 1];
                            //printf("%s", "ADD");
                            break;

                        // SUB: Subtracts two numbers
                        case 3:
                            SP -= 1;
                            stack[SP] = stack[SP] - stack[SP + 1];
                            //printf("%s", "SUB");
                            break;

                        // MUL: Multiplies two numbers
                        case 4:
                            SP -= 1;
                            stack[SP] = stack[SP] * stack[SP + 1];
                            //printf("%s", "MUL");
                            break;

                        // DIV: Divides two numbers
                        case 5:
                            SP -= 1;
                            stack[SP] = stack[SP] / stack[SP + 1];
                            //printf("%s", "DIV");
                            break;

                        // ODD: Determines whether the value in question is an odd number or not
                        case 6:
                            stack[SP] = stack[SP] % 2;
                            //printf("%s", "ODD");
                            break;

                        // MOD: Gets the remainder
                        case 7:
                            SP -= 1;
                            stack[SP] = stack[SP] % stack[SP + 1];
                            //printf("%s", "MOD");
                            break;

                        // EQL
                        case 8:
                            SP -= 1;
                            stack[SP] = stack[SP] == stack[SP + 1];
                            //printf("%s", "EQL");
                            break;

                        // NEQ:
                        case 9:
                            SP -= 1;
                            stack[SP] = stack[SP] != stack[SP + 1];
                            //printf("%s", "NEQ");
                            break;

                        // LSS:
                        case 10:
                            SP -= 1;
                            stack[SP] = stack[SP] < stack[SP + 1];
                            //printf("%s", "LSS");
                            break;

                        // LEQ:
                        case 11:
                            SP -= 1;
                            stack[SP] = stack[SP] <= stack[SP + 1];
                            //printf("%s", "LEQ");
                            break;

                        // GTR:
                        case 12:
                            SP -= 1;
                            stack[SP] = stack[SP] > stack[SP + 1];
                            //printf("%s", "GTR");
                            break;

                        // GEQ:
                        case 13:
                            SP -= 1;
                            stack[SP] = stack[SP] >= stack[SP + 1];
                            //printf("%s", "GEQ");
                            break;

                        default:
                            printf("Should not be here!");
                            break;
                    }
                }

                break;

            // LOD: Loads a variable from the stack into a given spot within the stack. This spot can be one the current
            // Activation Register or in a different Activation Register.
            case 3:
                //printf("%d  %s", PC - 1, "LOD");
                stackStore[arrayIterator].PC = PC - 1;
                strcpy(stackStore[arrayIterator].op, "LOD");

                SP += 1;
                stack[SP] = stack[base(stack, IR.l, BP) + IR.m];

                break;

            // STO: Loads a value at the top of the stack into some index in the stack
            case 4:
                //printf("%d  %s", PC - 1, "STO");
                stackStore[arrayIterator].PC = PC - 1;
                strcpy(stackStore[arrayIterator].op, "STO");

                stack[base(stack, IR.l, BP) + IR.m] = stack[SP];
                SP -= 1;

                break;

            // CAL: Creates an activation register and shifts all the appropriate values
            case 5:
                //printf("%d  %s", PC - 1, "CAL");
                stackStore[arrayIterator].PC = PC - 1;
                strcpy(stackStore[arrayIterator].op, "CAL");

                stack[SP + 1] = base(stack, IR.l, BP);
                stack[SP + 2] = BP;
                stack[SP + 3] = PC;
                stack[SP + 4] = stack[SP];
                BP = SP + 1;
                PC = IR.m;

                // Adds the SP to the AR array to know where to print the divider
                AR_Seperator[AR_Sep_Height] = SP;
                AR_Sep_Height++;

                break;

            // INC: Increases the number of values in the stack, the Stack Pointer will then be moved to account
            // for this increase.
            case 6:
                //printf("%d  %s", PC - 1, "INC");
                stackStore[arrayIterator].PC = PC - 1;
                strcpy(stackStore[arrayIterator].op, "INC");
                // The Stack Pointer increases by the desired number
                SP = SP + IR.m;
                //count++;
                stackStore[arrayIterator].inc = 1;
                break;

            // JMP: Jumps to a specific instruction by changing the Program Counter to the value of IR.m
            case 7:
                //printf("%d  %s", PC - 1, "JMP");
                stackStore[arrayIterator].PC = PC - 1;
                strcpy(stackStore[arrayIterator].op, "JMP");
                // Program Counter jumps to the desired instruction
                PC = IR.m;

                break;

            // JPC:
            case 8:
                //printf("%d  %s", PC - 1, "JPC");
                stackStore[arrayIterator].PC = PC - 1;
                strcpy(stackStore[arrayIterator].op, "JPC");

                if(stack[SP] == 0) {
                    PC = IR.m;
                }
                SP -= 1;

                break;

            // SYS: This Function deals with outputting data, taking in data, or ending the program.
            case 9:

                //prints element at top of stack
                if(IR.m == 1) {
                    printf("Top of Stack Value: %d\n", stack[SP]);
                    SP -= 1;
                }

                //takes in user input and places input on top of the stack
                else if(IR.m == 2)
                {
                  printf("Please Enter an Integer: ");
                  scanf("%d", &input);
                  stack[SP + 1] = input;
                  SP++;
                }

                //ends program
                else if(IR.m == 3)
                  halt = 0;

                else
                  printf("Should not be here!");

                //printf("%d  %s", PC - 1, "SYS");
                stackStore[arrayIterator].PC = PC - 1;
                strcpy(stackStore[arrayIterator].op, "SYS");

                break;

            default:
                printf("Invalid Operation - Ending Program\n");
                return 0;
                break;
        }

        // Prints the data of an operation before printing the stack after that operation
        //printf("  %d  %d   %d   %d    %d    ", IR.l, IR.m, PC, BP, SP);
        instructionData[arrayIterator].l = IR.l;
        instructionData[arrayIterator].m = IR.m;
        stackStore[arrayIterator].BP = BP;
        stackStore[arrayIterator].SP = SP;
        arrayIterator++;

        //prints out stack elements if an increment has occured
        /*if(count > 0)
        {
            for(int i = 0; i <= SP; i++)
            {
                printf("%d ", stack[i]);

                // This sections checks to see whether there are any additional Activation Registers. If AR_SEP_HEIGHT
                // is greater than 0, the code below will print the  "| " after the specified Activation registers.
                if(AR_Sep_Height > 0 && IR.opcode != 5) {
                    for(int j = 0; j < AR_Sep_Height; j++) {
                        if(AR_Seperator[j] == i) {
                            printf("| ");
                        }
                    }
                }

            }
        }*/
        //printf("\n");
    }
    //printf("\n");
    printVM(stackStore, instructionData, arrayIterator, stack);
}
