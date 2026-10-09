#include <cassert>
#include <cstddef>
#include <iostream>
#include "circularBuffer.hpp"


void simpleTest()
{
    std::cout << "Simple" << '\n';
    CircularBuffer cb;
    for(size_t i = 0; i < 10; ++i)
    {
        bool pushRetVal = cb.Push(i);
        assert(pushRetVal);
    }
    std::cout << cb;

    bool overflowPush = cb.Push(42);
    assert(overflowPush == false);

}

void simplePushPop()
{

    std::cout << "PUSH POP" << '\n';
    CircularBuffer cb;
    for(size_t i = 0; i < 10; ++i)
    {
        bool pushRetVal = cb.Push(i);
        assert(pushRetVal);
    }
    assert(cb.size() == 10);
    std::cout << cb;
    uint8_t val = 0;
    for(size_t i = 0; i < 10; ++i)
    {
        val = -1;
        bool popRetVal = cb.Pop(&val);
        assert(popRetVal);
        assert(val == i);
    }
    std::cout << cb;
}

int main(void)
{
    simpleTest();
    simplePushPop();


    return 0;
}