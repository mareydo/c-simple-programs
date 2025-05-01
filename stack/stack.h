#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Simple Stack
 * Mainly for uints since -1 is used as Error return value
 */
typedef struct stack_s
{
    int height;
    int top;

    int* data;
}stack_t;

stack_t* initStack(int);
void freeStack(stack_t**);


int peek(stack_t*);
int push(stack_t*, int);
int pop(stack_t*);

int isFull(stack_t*);
int isEmpty(stack_t*);

void printStack(stack_t*);

#endif