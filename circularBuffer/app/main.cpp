#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include "circularBuffer/circularBuffer.hpp"
#include "circularBuffer/constants.hpp"

void simpleTest()
{
    std::cout << "Simple" << '\n';
    CircularBuffer cb;
    for(size_t i = 0; i < 10; ++i)
    {
        bool pushRetVal = cb.push(i);
        assert(pushRetVal);
    }
    std::cout << cb;

    bool overflowPush = cb.push(42);
    assert(overflowPush == false);

}

void simplePushPop()
{

    std::cout << "PUSH POP" << '\n';
    CircularBuffer cb;
    for(size_t i = 0; i < 10; ++i)
    {
        bool pushRetVal = cb.push(i);
        assert(pushRetVal);
    }
    assert(cb.size() == 10);
    std::cout << cb;
    uint8_t val = 0;
    for(size_t i = 0; i < 10; ++i)
    {
        val = -1;
        bool popRetVal = cb.pop(&val);
        assert(popRetVal);
        assert(val == i);
    }
    std::cout << cb;
}

void producentRun(CircularBuffer &cb)
{
    for(int i = 0; i < CircularBufferConstants::BUFFER_SIZE; ++i)
    {
        if(!cb.push(i))
        {
            std::cout << "Producent failed to push " << i << '\n';
        }
        else
        {
            std::cout << "Producent to pushed " << i << '\n';
        }
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void consumentRun(CircularBuffer &cb)
{
    while(true)
    {
        uint8_t readVal = 0;
        if(!cb.pop(&readVal))
        {
            std::cout << "Consument failed to Pop";
        }
        else
        {
            std::cout << "Consument poped " << unsigned(readVal) << '\n';
        }
        //std::this_thread::sleep_for(std::chrono::seconds(3));
    }

}

int main(void)
{
    std::cout << "Buffer size: " << CircularBufferConstants::BUFFER_SIZE << '\n';

    //simpleTest();
    //simplePushPop();

    CircularBuffer cb;
    std::thread producent = std::thread(producentRun, std::ref(cb));
    std::thread consument = std::thread(consumentRun, std::ref(cb));

    producent.join();
    consument.join();


    return 0;
}