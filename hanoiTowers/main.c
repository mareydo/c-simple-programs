#include <stdio.h>
#include <string.h>

#include "stack.h"

#define PRINT_STEP
#define TOWER_SIZE 3

#define TOWER_A_NAME "A_from"
#define TOWER_B_NAME "B_tmp"
#define TOWER_C_NAME "C_dest"

void printTowers(stack_t* A, stack_t* B, stack_t* C)
{
    printf("-----\n");
    if(strcmp(A->name, TOWER_A_NAME) ==0) {printStack(A);}
    else if(strcmp(B->name, TOWER_A_NAME) ==0) {printStack(B);}
    else {printStack(C);}
    
    if(strcmp(A->name, TOWER_B_NAME) ==0) {printStack(A);}
    else if(strcmp(B->name, TOWER_B_NAME) ==0) {printStack(B);}
    else {printStack(C);}

    if(strcmp(A->name, TOWER_C_NAME) ==0) {printStack(A);}
    else if(strcmp(B->name, TOWER_C_NAME) ==0) {printStack(B);}
    else {printStack(C);}
    printf("-----\n");
}


/**
 * Hanoi Algorithm A->C
    move n-1 blocks from A to B, using C.
    move last block from A to C, using B.
    move n-1 blocks from B to C, using A.
 */

void hanoi(int count, stack_t* from, stack_t* dest, stack_t* tmp)
{
    if(count == 0)
    {
        return;
    }
    else
    {
        //move n-1 blocks from A to B, using C.
        hanoi(count-1,from, tmp, dest);
        //move last block from A to C, using B.
        push(dest,pop(from));
        #ifdef PRINT_STEP
            printTowers(from, dest, tmp);
        #endif
        //move n-1 blocks from B to C, using A.
        hanoi(count-1,tmp, dest, from);
    }
}




int main(void)
{
    stack_t* stackA = initStack(TOWER_SIZE, TOWER_A_NAME);
    if(stackA == NULL)
    {
        fprintf(stderr, "stackA Not Initialized\n");
        return 1;
    }
    stack_t* stackB = initStack(TOWER_SIZE, TOWER_B_NAME);
    if(stackB == NULL)
    {
        fprintf(stderr, "stackB Not Initialized\n");
        return 1;
    }
    stack_t* stackC = initStack(TOWER_SIZE, TOWER_C_NAME);
    if(stackC == NULL)
    {
        fprintf(stderr, "stackC Not Initialized\n");
        return 1;
    }

    for(int i = TOWER_SIZE -1; i >= 0; --i)
    {
        push(stackA, i);
    }

    printf("Start\n");
    printTowers(stackA, stackB, stackC);

    hanoi(TOWER_SIZE, stackA, stackC, stackB);

    printf("Stop\n");
    printTowers(stackA, stackB, stackC);

    freeStack(&stackA);
    freeStack(&stackB);
    freeStack(&stackC);

    return 0;
}