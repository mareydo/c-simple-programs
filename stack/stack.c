#include "stack.h"

stack_t* initStack(int height)
{
    stack_t* stack = (stack_t*)malloc(sizeof(stack_t));
    if(stack == NULL) 
    {
        return NULL;
    }

    stack->height = height;
    stack->top = -1;

    stack->data = (int*)calloc(height, sizeof(int));
    if(stack->data == NULL)
    {
        free(stack);
        return NULL;
    }
    return stack;
}

void freeStack(stack_t** stack)
{
    if(!stack) { return; };

    free((*stack)->data);
    free(*stack);
    *stack = NULL;
}

int peek(stack_t* stack)
{
    if(!stack) { return -1; };

    if(isEmpty(stack)) 
    {
        return -1;
    }
    return stack->data[stack->top];
}

int push(stack_t* stack, int newValue)
{
    if(!stack) { return -1; }

    if(isFull(stack)) { return -1; }
    stack->top += 1;
    stack->data[stack->top] = newValue;
    return 1;
}
int pop(stack_t* stack) 
{
    if(!stack) return -1;

    if(isEmpty(stack))
    {
        return -1;
    }
    int retVal = stack->data[stack->top];
    stack->data[stack->top] = 0;
    stack->top -= 1;
    return retVal;
}

int isFull(stack_t*stack)
{
    return (stack->top == stack->height -1);
}

int isEmpty(stack_t*stack)
{
    return (stack->top == -1);
}

void printStack(stack_t* stack)
{
    for(int i = 0; i <= stack->top; ++i)
    {
        printf("%d", stack->data[i]);
        if(i != stack->top)
        {
            printf(",");
        }
    }
    printf("\n");
}