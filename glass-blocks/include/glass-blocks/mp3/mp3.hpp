#include <cstdint>
#include "glass-blocks/serial/serial.hpp"

class MP3 : public Serial
{
    private:
        uint8_t id;
    public:
        MP3(const uint8_t id);
        bool play();
        bool pause();
        bool volumeUp();
        bool volumeDown();
        uint8_t getVolume();
};