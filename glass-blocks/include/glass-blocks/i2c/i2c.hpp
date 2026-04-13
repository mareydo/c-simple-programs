#include <cstdint>
#include <cstddef>

class I2C
{
    private:
        int handle;
        uint8_t address;
        uint8_t adapterNr;

        bool isActive();
        void disconnect();
    public:
        I2C(const uint8_t address,const uint8_t adapterNr);
        ~I2C();
        bool init();
        bool writeByte(const uint8_t command);
        bool writeBytes(const uint8_t* commands, const size_t len);
        bool readByte(const uint8_t command, uint8_t& data);
};