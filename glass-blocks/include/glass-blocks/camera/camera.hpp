#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <cstdint>
#include <iostream>

#include "glass-blocks/i2c/i2c.hpp"
#include "glass-blocks/camera/MLX90642_commands.hpp"

class Camera
{
private:
    I2C i2c;

    uint8_t     height;
    uint8_t     width;
    uint16_t*    pixelBuffer;


    bool singleAddressWriteCommand(const uint16_t opcode, const uint16_t command);
    void sleepMs(const uint32_t ms);
    bool writeConfigReg(const uint16_t configAddress, const uint16_t data);
    bool setConfig(const uint16_t configAddress, const uint16_t mask, const uint16_t data);
    uint16_t readConfigReg(const uint16_t configAddress);
    uint16_t readConfigReg(const uint16_t configAddress, const uint16_t mask);
    uint16_t readDataFlags();
    uint16_t readDataFlags(const uint16_t mask);
public:
    struct CameraVersion 
    {
        uint8_t    major;
        uint8_t    minor;
        uint8_t    patch;
    };

    Camera(const uint8_t width, const uint8_t height, const uint8_t address, const uint8_t adapterNr);
    ~Camera();
    bool init();
    CameraVersion readVersion();
    uint16_t getRefreshRate();
    bool setRefreshRate(const uint16_t refreshRate);
    uint16_t getEmissivity();
    bool setEmissivity(const uint16_t emissivity);
    uint16_t getMeasuringMode();
    bool setMeasuringMode(const uint16_t mode);
    uint16_t getOutputDataFormat();
    bool setOutputDataFormat(const uint16_t dataFormat);
    uint16_t getReferredUThreshold();
    bool setReferredUThreshold(const uint16_t thresholdMode);
    uint16_t getCurrentLimit();
    bool setCurrentLimit(const uint16_t currentLimit);
    uint16_t getFMMode();
    bool setFMMode(const uint16_t fmMode);
    bool setI2CAddr(const uint8_t address);
    uint16_t getBackgroundTemp();
    bool setBackgroundTemp(const uint16_t temp);
    uint8_t readProgressBar();
    uint16_t readFrameUpdateFlag();
    uint16_t readReadyFlag();
    uint16_t readBusyFlag();
    bool isReadyFlag();
    bool update();
    void printBufferRaw() const;
    void printBufferCelsius() const;


private:
    CameraVersion version;
};

#endif /* _CAMERA_HPP_ */