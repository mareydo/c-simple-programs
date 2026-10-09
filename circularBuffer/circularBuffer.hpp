#pragma once

#include <cstdint>
#include <ostream>


class CircularBuffer
{
    #define BUFFER_SIZE 10
    private:
        uint8_t buffer[BUFFER_SIZE];
        std::size_t head;
        std::size_t tail;
        std::size_t bufferCounter;

        bool isFull();
        bool isEmpty();
        void moduloIterate(std::size_t&);
    public:
        explicit CircularBuffer();
        bool Push(const uint8_t);
        bool Pop(uint8_t*);
        bool Peek(uint8_t*);
        size_t size();

        friend std::ostream& operator<<(std::ostream&,CircularBuffer&);
};