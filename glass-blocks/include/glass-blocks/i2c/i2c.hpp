#ifndef _I2C_HPP_
#define _I2C_HPP_

#include <cstdint>
#include <cstddef>

class I2C
{
    private:
        int handle;
        uint8_t address;
        uint8_t adapterNr;

        bool isActive();
public:
        I2C(const uint8_t address,const uint8_t adapterNr);
        ~I2C();
        void disconnect();
        bool init();
        bool write8(const uint8_t command);
        bool write16(const uint16_t command);
        bool writeN(const uint8_t* commands, const size_t len);
        bool read16(uint16_t* buffer);
        bool readN(uint8_t* buffer, const size_t len);
        bool RAWwriteNreadM(uint8_t* writeBuffer, const uint16_t N, uint8_t* readBuffer, const uint16_t M);
        bool RAWwrite16read16(uint16_t command, uint16_t* buffer);
};

#endif /* _I2C_HPP_ */