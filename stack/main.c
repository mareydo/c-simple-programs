#include <stdio.h>
#include <assert.h>

#include "stack.h"

/**
 * @brief Fills stack, prints content, frees it
 * 
 */
void simpleTest()
{
    stack_t* stack = initStack(4);
    if(stack == NULL)
    {
        fprintf(stderr, "simpleTest: Stack Not Initialized\n");
    }
    assert(isEmpty(stack) == 1);
    assert(isFull(stack) == 0);
    assert(push(stack, 1) == 1);
    assert(push(stack, 2) == 1);
    assert(push(stack, 3) == 1);
    assert(push(stack, 4) == 1);
    assert(isEmpty(stack) == 0);
    assert(isFull(stack) == 1);
    assert(push(stack, 5) == -1);   //push into full
    printf("Simple test (1,2,3,4) : ");
    printStack(stack);
    freeStack(&stack);
    assert(pop(stack) == -1);   //pop from empty 

    assert(stack == NULL);
}

/**
 * @brief Fills stack, pops all values, frees it
 * 
 */
void unloadTest()
{
    int SIZE = 4;
    stack_t* stack = initStack(SIZE);
    if(stack == NULL)
    {
        fprintf(stderr, "unloadTest: Stack Not Initialized\n");
    }
    for(int i = 0; i < SIZE; ++i)
    {
        assert(push(stack, i) == 1); 
        assert(peek(stack) == i);
    }
    assert(isEmpty(stack) == 0);
    assert(isFull(stack) == 1);

    printf("Unload test (0,1,2,3): ");
    printStack(stack);
    for(int i = 0; i < SIZE; ++i)
    {
        assert(peek(stack) == SIZE - i -1);
        assert(pop(stack) == SIZE - i -1); 
    }
    assert(isEmpty(stack) == 1);
    assert(isFull(stack) == 0);

    printf("Unload test '': ");
    printStack(stack);

    freeStack(&stack);
    assert(stack == NULL);
}

int main(void)
{
    simpleTest(); 
    unloadTest();

    return 0;    
}