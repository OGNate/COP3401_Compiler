/*
    Nathan Wilson and Dillon Garcia
    HW1 P-Machines
    COP3402 -> Systems Software -> Spring 2021
    Professor Montagne
*/
#include "VM.h"

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

void VM_Main(instruction *VM_Instructions, int V_Directive) {
    instruction IR;

    // Creates an array called text of type Instruction_Register with a max length of MAX_CODE_LENGTH
    instruction text[MAX_CODE_LENGTH];

    // Creates our stack array called "stack" of type int with a max length of MAX_STACK_HEIGHT
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

    int VM_Counter = 0; // Virtual Machine Counter

    // Copies all the VM_Instruction from the Parser Codegen to the Virtual Machine
    while(VM_Instructions[VM_Counter].opcode != 0) {
        text[textIncrementor].opcode = VM_Instructions[VM_Counter].opcode;
        text[textIncrementor].l = VM_Instructions[VM_Counter].l;
        text[textIncrementor].m = VM_Instructions[VM_Counter].m;
        VM_Counter++;
        textIncrementor++;
    }

    // Setting up Initial Values
    int SP = -1;
    int BP = 0;
    int PC = 0;
    int count = 0;
    int input;

    // Halt flag when set to 0 will end the program
    int halt = 1;

    if(V_Directive == 1) {
        // Printing the initial values of the program variables
        printf("\t\tPC   BP   SP   stack\n");
        printf("Initial values: %d    %d    %d\n", PC, BP, SP);
    }

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
                strcpy(IR.op, "LIT");
                if(V_Directive == 1) printf("%d  %s", PC - 1, IR.op);

                SP += 1;
                stack[SP] = IR.m;

                break;

            // OPR: Performs some type of operation
            case 2:
                // Only prints if the V_Directive is active
                if(V_Directive == 1) printf("%d  ", PC - 1);

                // Checks to make sure that IR.m is within the given bounds
                if((IR.m >= 0) && (IR.m <= 13)) {
                    strcpy(IR.op, "OPR");
                    switch(IR.m) {

                        // RTN: Returns one activation register down.
                        case 0:
                            stack[BP - 1] = stack[SP];
                            SP = BP - 1;
                            BP = stack[SP + 2];
                            PC = stack[SP + 3];
                            //strcpy(IR.op, "RTN");
                            if(V_Directive == 1) printf("%s", IR.op);

                            // Reduces the height of the AR_Seperator array
                            AR_Sep_Height--;

                            break;

                        // NEG: returns the negative number of what is in stack[SP]
                        case 1:
                            stack[SP] = -1 * stack[SP];
                            //strcpy(IR.op, "NEG");
                            if(V_Directive == 1) printf("%s", IR.op);
                            break;

                        // ADD: Adds two numbers
                        case 2:
                            SP -= 1;
                            stack[SP] = stack[SP] + stack[SP + 1];
                            //strcpy(IR.op, "ADD");
                            if(V_Directive == 1) printf("%s", IR.op);
                            break;

                        // SUB: Subtracts two numbers
                        case 3:
                            SP -= 1;
                            stack[SP] = stack[SP] - stack[SP + 1];
                            //strcpy(IR.op, "SUB");
                            if(V_Directive == 1) printf("%s", IR.op);
                            break;

                        // MUL: Multiplies two numbers
                        case 4:
                            SP -= 1;
                            stack[SP] = stack[SP] * stack[SP + 1];
                            //strcpy(IR.op, "MUL");
                            if(V_Directive == 1) printf("%s", IR.op);
                            break;

                        // DIV: Divides two numbers
                        case 5:
                            SP -= 1;
                            stack[SP] = stack[SP] / stack[SP + 1];
                            //strcpy(IR.op, "DIV");
                            if(V_Directive == 1) printf("%s", IR.op);
                            break;

                        // ODD: Determines whether the value in question is an odd number or not
                        case 6:
                            stack[SP] = stack[SP] % 2;
                            //strcpy(IR.op, "ODD");
                            if(V_Directive == 1) printf("%s", IR.op);
                            break;

                        // MOD: Gets the remainder
                        case 7:
                            SP -= 1;
                            stack[SP] = stack[SP] % stack[SP + 1];
                            //strcpy(IR.op, "MOD");
                            if(V_Directive == 1) printf("%s", IR.op);
                            break;

                        // EQL
                        case 8:
                            SP -= 1;
                            stack[SP] = stack[SP] == stack[SP + 1];
                            //strcpy(IR.op, "EQL");
                            if(V_Directive == 1) printf("%s", IR.op);
                            break;

                        // NEQ:
                        case 9:
                            SP -= 1;
                            stack[SP] = stack[SP] != stack[SP + 1];
                            //strcpy(IR.op, "NEQ");
                            if(V_Directive == 1) printf("%s", IR.op);
                            break;

                        // LSS:
                        case 10:
                            SP -= 1;
                            stack[SP] = stack[SP] < stack[SP + 1];
                            //strcpy(IR.op, "LSS");
                            if(V_Directive == 1) printf("%s", IR.op);
                            break;

                        // LEQ:
                        case 11:
                            SP -= 1;
                            stack[SP] = stack[SP] <= stack[SP + 1];
                            //strcpy(IR.op, "LEQ");
                            if(V_Directive == 1) printf("%s", IR.op);
                            break;

                        // GTR:
                        case 12:
                            SP -= 1;
                            stack[SP] = stack[SP] > stack[SP + 1];
                            //strcpy(IR.op, "GTR");
                            if(V_Directive == 1) printf("%s", IR.op);
                            break;

                        // GEQ:
                        case 13:
                            SP -= 1;
                            stack[SP] = stack[SP] >= stack[SP + 1];
                            //strcpy(IR.op, "GEQ");
                            if(V_Directive == 1) printf("%s", IR.op);
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
                strcpy(IR.op, "LOD");
                if(V_Directive == 1) printf("%d  %s", PC - 1, IR.op);

                SP += 1;
                stack[SP] = stack[base(stack, IR.l, BP) + IR.m];

                break;

            // STO: Loads a value at the top of the stack into some index in the stack
            case 4:
                strcpy(IR.op, "STO");
                if(V_Directive == 1) printf("%d  %s", PC - 1, IR.op);

                stack[base(stack, IR.l, BP) + IR.m] = stack[SP];
                SP -= 1;

                break;

            // CAL: Creates an activation register and shifts all the appropriate values
            case 5:
                strcpy(IR.op, "CAL");
                if(V_Directive == 1) printf("%d  %s", PC - 1, IR.op);

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
                strcpy(IR.op, "INC");
                if(V_Directive == 1) printf("%d  %s", PC - 1, IR.op);
                // The Stack Pointer increases by the desired number
                SP = SP + IR.m;
                count++;
                break;

            // JMP: Jumps to a specific instruction by changing the Program Counter to the value of IR.m
            case 7:
                strcpy(IR.op, "JMP");
                if(V_Directive == 1) printf("%d  %s", PC - 1, IR.op);
                // Program Counter jumps to the desired instruction
                PC = IR.m;

                break;

            // JPC: 
            case 8:
                strcpy(IR.op, "JPC");
                if(V_Directive == 1) printf("%d  %s", PC - 1, IR.op);

                if(stack[SP] == 0) {
                    PC = IR.m;
                }
                SP -= 1;

                break;

            // SYS: This Function deals with outputting data, taking in data, or ending the program.
            case 9:

                strcpy(IR.op, "SYS");

                //prints element at top of stack
                if(IR.m == 1) {
                    if(V_Directive == 1) printf("Top of Stack Value: %d\n", stack[SP]);
                    SP -= 1;
                }

                //takes in user input and places input on top of the stack
                else if(IR.m == 2)
                {
                  if(V_Directive == 1) printf("Please Enter an Integer: ");
                  scanf("%d", &input);
                  stack[SP + 1] = input;
                  SP++;
                }

                //ends program
                else if(IR.m == 3)
                  halt = 0;

                else
                  printf("Should not be here!");

                if(V_Directive == 1) printf("%d  %s", PC - 1, IR.op);

                break;

            default:
                if(V_Directive == 1) printf("Invalid Operation - Ending Program\n");
                break;
        }

        if(V_Directive == 1) {
            // Prints the data of an operation before printing the stack after that operation
            printf("  %d  %d   %d   %d    %d    ", IR.l, IR.m, PC, BP, SP);

            //prints out stack elements if an increment has occured
            if(count > 0)
            {
                for(int i = 0; i <= SP; i++)
                {
                    printf("%d ", stack[i]);

                    // This sections checks to see whether there are any additional Activation Registers. If AR_SEP_HEIGHT
                    // is greater than 0, the code below will print the  "| " after the specified Activation registers.
                    if(AR_Sep_Height > 0 && strcmp(IR.op, "CAL") != 0) {
                        for(int j = 0; j < AR_Sep_Height; j++) {
                            if(AR_Seperator[j] == i) {
                                printf("| ");
                            }
                        }
                    }

                }
            }
            printf("\n");
        }
    }
    printf("\n");
}