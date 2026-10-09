#include "circularBuffer.hpp"
#include <cstddef>

CircularBuffer::CircularBuffer(): head(0), tail(0), bufferCounter(0)
{}

bool CircularBuffer::isFull()
{
    //This waste one element return ((head+1)%BUFFER_SIZE) == tail;
    return bufferCounter == BUFFER_SIZE;
}
bool CircularBuffer::isEmpty()
{
    //return head == tail;
    return bufferCounter == 0;
}

void CircularBuffer::moduloIterate(std::size_t& ptr)
{
    ++ptr;
    ptr %= BUFFER_SIZE;
}

bool CircularBuffer::Push(const uint8_t val)
{
    // check
    if(isFull()) { return false; }
    // write
    buffer[tail] = val;
    ++bufferCounter;
    // move head
    moduloIterate(tail);

    return true;
}
bool CircularBuffer::Pop(uint8_t *val)
{
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

bool CircularBuffer::Peek(uint8_t *val)
{
    // check
    if(isEmpty()) { return false; }
    if(val == nullptr) { return false; }
    // read
    *val = buffer[head];

    return true;
}

size_t CircularBuffer::size()
{
    return bufferCounter;
}

std::ostream& operator<<(std::ostream &os,CircularBuffer &buf)
{
    os << "Available: " << BUFFER_SIZE << '\n';
    //size_t realSize = (buf.head >= buf.tail) ? buf.head-buf.tail : BUFFER_SIZE-buf.tail + buf.head;
    os << "Used: " << unsigned(buf.bufferCounter) << '\n';
    os << "IsFull: " << buf.isFull() << " IsEmpty: " << buf.isEmpty() << '\n';
    for(size_t i = 0; i < BUFFER_SIZE; ++i)
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

        if (i+1 != BUFFER_SIZE)
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