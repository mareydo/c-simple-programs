#include <cstdio>

class I2C
{
    private:
    protected:
        uint8_t address;

        int writeByte(const uint8_t address, const uint8_t data);
        int readByte(const uint8_t address, uint8_t& data);
    public:
}