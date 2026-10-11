#pragma once

#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <ostream>

#include "constants.hpp"


class CircularBuffer
{
    private:
        uint8_t buffer[CircularBufferConstants::BUFFER_SIZE];
        std::size_t head;
        std::size_t tail;
        std::size_t bufferCounter;

        std::mutex mtx;
        std::condition_variable valueAvailable;

        bool isFull();
        bool isEmpty();
        void moduloIterate(std::size_t&);
    public:
        explicit CircularBuffer();
        bool push(const uint8_t);
        bool pop(uint8_t*);
        bool peek(uint8_t*);
        size_t size();

        friend std::ostream& operator<<(std::ostream&,CircularBuffer&);
};