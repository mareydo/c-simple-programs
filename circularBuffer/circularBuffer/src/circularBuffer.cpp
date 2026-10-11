#include "circularBuffer/circularBuffer.hpp"

#include <cstddef>
#include <iostream>
#include <mutex>

CircularBuffer::CircularBuffer(): head(0), tail(0), bufferCounter(0)
{}

bool CircularBuffer::isFull()
{
    // !Warning! Not thread safe, only public API is guarded

    //This waste one element return ((head+1)%BUFFER_SIZE) == tail;
    return bufferCounter == CircularBufferConstants::BUFFER_SIZE;
}
bool CircularBuffer::isEmpty()
{
    // !Warning! Not thread safe, only public API is guarded

    //return head == tail;
    return bufferCounter == 0;
}

void CircularBuffer::moduloIterate(std::size_t& ptr)
{
    // !Warning! Not thread safe, only public API is guarded

    ++ptr;
    ptr %= CircularBufferConstants::BUFFER_SIZE;
}

bool CircularBuffer::push(const uint8_t val)
{
    // prevents busy wait
    std::unique_lock<std::mutex> lg(mtx);
    valueAvailable.wait(lg, [this]() { return ! isFull(); });

    // check
    // if(isFull()) { return false; }
    // write
    buffer[tail] = val;
    ++bufferCounter;
    // move head
    moduloIterate(tail);
    valueAvailable.notify_all();

    return true;
}
bool CircularBuffer::pop(uint8_t *val)
{

    std::unique_lock<std::mutex> lg(mtx);
    valueAvailable.wait(lg, [this]() { return ! isEmpty(); });
    // check
    if(isEmpty()) { return false; }
    if(val == nullptr) { return false; }
    // read
    *val = buffer[head];
    buffer[head] = 0xff;
    --bufferCounter;
    // move tail
    moduloIterate(head);

    return true;
}

bool CircularBuffer::peek(uint8_t *val)
{
    std::lock_guard<std::mutex> lg(mtx);
    // check
    if(isEmpty()) { return false; }
    if(val == nullptr) { return false; }
    // read
    *val = buffer[head];

    return true;
}

size_t CircularBuffer::size()
{
    std::lock_guard<std::mutex> lg(mtx);
    return bufferCounter;
}

std::ostream& operator<<(std::ostream &os,CircularBuffer &buf)
{
    std::lock_guard<std::mutex> lg(buf.mtx);
    os << "Available: " << CircularBufferConstants::BUFFER_SIZE << '\n';
    //size_t realSize = (buf.head >= buf.tail) ? buf.head-buf.tail : BUFFER_SIZE-buf.tail + buf.head;
    os << "Used: " << unsigned(buf.bufferCounter) << '\n';
    os << "IsFull: " << buf.isFull() << " IsEmpty: " << buf.isEmpty() << '\n';
    for(size_t i = 0; i < CircularBufferConstants::BUFFER_SIZE; ++i)
    {
        os << unsigned(buf.buffer[i]);

        if(i == buf.head)
        {
            os << 'H';
        }
        if (i == buf.tail)
        {
            os << 'T';
        }

        if (i+1 != CircularBufferConstants::BUFFER_SIZE)
        {
            os << ',';
        }
        else
        {
            os << '\n';
        }
    }
    return os;
}