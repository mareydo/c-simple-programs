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

bool I2C::writeByte(const uint8_t command)
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

bool I2C::writeBytes(const uint8_t* commands, const size_t len)
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

bool I2C::readByte(const uint8_t command, uint8_t& data)
{
    if(!isActive())
    {
        std::cerr << "Error reading byte: " << "Connection closed" << std::endl;
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