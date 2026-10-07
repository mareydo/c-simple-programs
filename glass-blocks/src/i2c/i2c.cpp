#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h> 
#include <iostream>
#include <string>

#include "glass-blocks/i2c/i2c.hpp"

I2C::I2C(const uint8_t address, const uint8_t adapterNr): handle(-1), address(address), adapterNr(adapterNr)
{}

I2C::~I2C()
{
    disconnect();
}

bool I2C::init()
{
    std::cout << "I2C init" << std::endl;
    std::string filename = "/dev/i2c-" + std::to_string(adapterNr);
    handle = open(filename.c_str(), O_RDWR);
    if (handle < 0) 
    {
        std::cerr << "Cannot open I2C adapter: " << unsigned(adapterNr) << std::endl;
        return false;
    }

    if (ioctl(handle, I2C_SLAVE, address) < 0)
    {
        std::cerr << "Cannot connect to I2C slave at: " << unsigned(address) << std::endl;
        disconnect();
        return false;
    }

    std::cout << "I2C opened" << std::endl;
    return true;

}

bool I2C::write8(const uint8_t command)
{
    if(!isActive())
    {
        std::cerr << "Error sending byte: " << "Connection closed" << std::endl;
        return false;
    }
    if (write(handle, &command, 1) != 1) 
    {
        std::cerr << "Error sending byte: " << unsigned(command) << std::endl;
        disconnect();
        return false;
    }
    return true;
}

bool I2C::write16(const uint16_t command)
{
    if(!isActive())
    {
        std::cerr << "Error sending two bytes: " << "Connection closed" << std::endl;
        return false;
    }
    uint8_t arr[2];
    arr[0] = (command >> 8) & 0xFF;
    arr[1] = command & 0xFF;
    if (write(handle, arr, 2) != 2) 
    {
        std::cerr << "Error sending two bytes" << std::endl;
        disconnect();
        return false;
    }
    return true;
}

bool I2C::writeN(const uint8_t* commands, const size_t len)
{
    if(!isActive())
    {
        std::cerr << "Error sending bytes: " << "Connection closed" << std::endl;
        return false;
    }
    if(write(handle, commands, len) != len)
    {
        std::cerr << "Error sending bytes of length: " << unsigned(len) << std::endl;
        disconnect();
        return false;
    }
    return true;
}

bool I2C::read16(uint16_t* buffer)
{
    if(!isActive())
    {
        std::cerr << "Error reading byte: " << "Connection closed" << std::endl;
        return false;
    }
    uint8_t arr[2] = {0};

    if(read(handle, &arr, 2) != 2)
    {
        std::cerr << "Error reading two bytes" << std::endl;
        disconnect();
        return false;
    }
    *buffer = arr[0]<<8 | arr[1];
    return true;
}

bool I2C::readN(uint8_t* buffer, const size_t len)
{
    if(!isActive())
    {
        std::cerr << "Error reading byte: " << "Connection closed" << std::endl;
        return false;
    }

    if(read(handle, buffer, len) != len)
    {
        std::cerr << "Error reading bytes of length: " << unsigned(len) << std::endl;
        disconnect();
        return false;
    }
    return true;
}

bool I2C::isActive()
{
    return handle >= 0;
}

void I2C::disconnect()
{
    if(isActive())
    {
        close(handle);
        handle = -1;
    }
}


bool I2C::RAWwriteNreadM(uint8_t* writeBuffer, const uint16_t N, uint8_t* readBuffer, const uint16_t M)
{
    i2c_msg msgs[2] = 
    {
        { address, 0,        N, writeBuffer },
        { address, I2C_M_RD, M, readBuffer }
    };
    i2c_rdwr_ioctl_data data = { msgs, 2 };

    if ( ioctl(handle, I2C_RDWR, &data) < 0) 
    {
        std::cerr << "Error reading RAW W16R16 with errcode: " << errno  << std::endl;
        disconnect();
        return false;
    }
    return true;
}


bool I2C::RAWwrite16read16(uint16_t command, uint16_t* buffer)
{
    uint8_t writeBuffer[2];
    writeBuffer[0] = command >> 8 & 0xFF;
    writeBuffer[1] = command & 0xFF;

    uint8_t readBuffer[2] = {0};
    if(!RAWwriteNreadM(writeBuffer, 2, readBuffer, 2))
    {
        return false;
    }

    *buffer = (readBuffer[0] << 8) | readBuffer[1];
    return true;
}

