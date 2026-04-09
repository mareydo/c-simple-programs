#include <cstdint>

class I2C
{
    private:
    protected:
        uint8_t address;

        int writeByte(const uint8_t command);
        int readByte(const uint8_t command, uint8_t& data);
    public:
};