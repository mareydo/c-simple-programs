#include <cassert>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <thread>
#include "circularBuffer/circularBuffer.hpp"
#include "circularBuffer/constants.hpp"
#include "version.h"


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
    // TODO mabe jthread or other better mechanism to break the loop
    do
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
        if(cb.size()==0) { break; }
    }
    while(true);

}

int main(int argc, char* argv[])
{
    if (argc > 1 && std::strcmp(argv[1], "--version") == 0) {
        std::cout << "CircularBuffer "
                    <<  CB_VERSION_STRING
                    << '\n';
        return 0;
    }

    std::cout << "Buffer size: " << CircularBufferConstants::BUFFER_SIZE << '\n';

    CircularBuffer cb;
    std::thread producent = std::thread(producentRun, std::ref(cb));
    std::thread consument = std::thread(consumentRun, std::ref(cb));

    producent.join();
    consument.join();

    return 0;
}