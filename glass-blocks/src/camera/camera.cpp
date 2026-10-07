#include "glass-blocks/camera/camera.hpp"

#include <chrono>
#include <thread>
#include <iomanip>

Camera::Camera(const uint8_t width, const uint8_t height, const uint8_t address, const uint8_t adapterNr) : i2c(address,adapterNr)
{
    this->width = width;
    this->height = height;
    this->pixelBuffer = new uint8_t[width * ((height+7)/8)];

    this->version.major=0;
    this->version.minor=0;
    this->version.patch=0;

}
Camera::~Camera()
{
    delete[] pixelBuffer;
    i2c.disconnect();
}

void Camera::sleepMs(const uint32_t ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}



bool Camera::init()
{
    std::cout << "OLED init" << std::endl;
    if(!i2c.init())
    {
        return false;
    }
    return true;
}

Camera::CameraVersion Camera::readVersion()
{
    if(version.major != 0)
    {
        return version;
    }

    uint16_t response;
    if(!i2c.RAWwrite16read16(REG_FIRMWARE_MAJOR,&response))
    {
        version.major = 0;  //zero to trigger reading again
        return {0,0,0};
    }
    version.major = response >> 8;
    if(!i2c.RAWwrite16read16(REG_FIRMWARE_MINOR_PATCH,&response))
    {
        version.major = 0; //zero to trigger reading again
        return {0,0,0};
    }
    version.minor = response;
    version.patch = response >> 8;

    return version;
}

uint16_t Camera::readConfigReg(const uint16_t configAddress)
{
    uint16_t response;
    if(!i2c.RAWwrite16read16(configAddress, &response))
    {
        return INVALID_VALUE;
    }
    return response;
}
uint16_t Camera::readConfigReg(const uint16_t configAddress, const uint16_t mask)
{
    uint16_t raw = readConfigReg(configAddress);
    if(raw == INVALID_VALUE)
    {
        return INVALID_VALUE;
    }
    return raw & mask;
}


bool Camera::writeConfigReg(const uint16_t configAddress, const uint16_t data)
{
    uint16_t reg = readConfigReg(configAddress);
    if(reg == INVALID_VALUE)
    {
        return false;
    }
    uint8_t buffer[6];
    buffer[0] = (CONFIGURATION_CMD >> 8) & 0xFF;
    buffer[1] = CONFIGURATION_CMD & 0XFF;
    buffer[2] = configAddress >> 8;
    buffer[3] = configAddress;
    buffer[4] = data >> 8;
    buffer[5] = data;
    return i2c.writeN(buffer,6);
}

bool Camera::setConfig(const uint16_t configAddress, const uint16_t mask, const uint16_t data)
{
    uint16_t config = readConfigReg(configAddress);
    if(config == INVALID_VALUE)
    {
        return false;
    }
    config &= ~mask;
    config |= data;
    writeConfigReg(configAddress,config);
    sleepMs(15);
    return true;
}

uint16_t Camera::getRefreshRate()
{
    return readConfigReg(CONFIG_ADDR_REFRESH_RATE_CONFIG,REFRESH_RATE_MASK);
}
bool Camera::setRefreshRate(const uint16_t refreshRate)
{
    return setConfig(CONFIG_ADDR_REFRESH_RATE_CONFIG,
                    REFRESH_RATE_MASK,
                    refreshRate) != INVALID_VALUE;
}

uint16_t Camera::getEmissivity()
{
    return readConfigReg(CONFIG_ADDR_EMISSIVITY,EMISSIVITY_MASK);
}
bool Camera::setEmissivity(const uint16_t emissivity)
{
    return setConfig(CONFIG_ADDR_EMISSIVITY,
                    EMISSIVITY_MASK,
                    emissivity) != INVALID_VALUE;
}

uint16_t Camera::getMeasuringMode()
{
    return readConfigReg(CONFIG_ADDR_APPLICATION_CONFIG,MEASURING_MODE_MASK);
}
bool Camera::setMeasuringMode(const uint16_t mode)
{
    return setConfig(CONFIG_ADDR_APPLICATION_CONFIG,
                    MEASURING_MODE_MASK,
                    mode) != INVALID_VALUE;
}

uint16_t Camera::getOutputDataFormat()
{
    return readConfigReg(CONFIG_ADDR_APPLICATION_CONFIG,OUTPUT_DATA_FORMAT_MASK);
}
bool Camera::setOutputDataFormat(const uint16_t dataFormat)
{
    return setConfig(CONFIG_ADDR_APPLICATION_CONFIG,
                    OUTPUT_DATA_FORMAT_MASK,
                    dataFormat) != INVALID_VALUE;
}

uint16_t Camera::getReferredUThreshold()
{
    return readConfigReg(CONFIG_ADDR_I2C_ANALOG_CONFIG,REFERRED_U_THRESHOLD_MASK);
}
bool Camera::setReferredUThreshold(const uint16_t thresholdMode)
{
    return setConfig(CONFIG_ADDR_I2C_ANALOG_CONFIG,
                    REFERRED_U_THRESHOLD_MASK,
                    thresholdMode) != INVALID_VALUE;
}
uint16_t Camera::getCurrentLimit()
{
    return readConfigReg(CONFIG_ADDR_I2C_ANALOG_CONFIG,CURRENT_LIMIT_MASK);
}
bool Camera::setCurrentLimit(const uint16_t currentLimit)
{
    return setConfig(CONFIG_ADDR_I2C_ANALOG_CONFIG,
                    CURRENT_LIMIT_MASK,
                    currentLimit) != INVALID_VALUE;
}
uint16_t Camera::getFMMode()
{
    return readConfigReg(CONFIG_ADDR_I2C_ANALOG_CONFIG,FM_MODE_MASK);
}
bool Camera::setFMMode(const uint16_t fmMode)
{
    return setConfig(CONFIG_ADDR_I2C_ANALOG_CONFIG,
                    FM_MODE_MASK,
                    fmMode) != INVALID_VALUE;
}

bool Camera::setI2CAddr(const uint8_t address)
{
    //TODO
    return true;
}

uint16_t Camera::getBackgroundTemp()
{
    return 0; //TODO
}
bool Camera::setBackgroundTemp(const uint16_t temp)
{
    return setConfig(CONFIG_ADDR_BACKGROUND_TEMP,
                    BACKGROUND_TEMP_MASK,
                    temp) != INVALID_VALUE;
}

uint8_t Camera::readProgressBar()
{
    //TODO
    return 0;
}

uint16_t Camera::readDataFlags()
{
    uint16_t response;
    if(!i2c.RAWwrite16read16(REG_DATA_FLAGS,&response))
    {
        return -1;
    }
    return response;
}
uint16_t Camera::readDataFlags(const uint16_t mask)
{
    uint16_t flags = readDataFlags();
    if(flags == INVALID_VALUE)
    {
        return INVALID_VALUE;
    }
    return flags & mask;
}

uint16_t Camera::readFrameUpdateFlag()
{
    return readDataFlags(FRAME_UPDATE_FLAG_MASK);
}
uint16_t Camera::readReadyFlag()
{
    return readDataFlags(READY_FLAG_MASK);
}
bool Camera::isReadyFlag()
{
    uint16_t flag = readReadyFlag();
    if(flag == INVALID_VALUE) return false;
    return flag == READY_FLAG_MASK;
}
uint16_t Camera::readBusyFlag()
{
    return readDataFlags(BUSY_FLAG_MASK);
}

bool Camera::update()
{
    //wait for READY 1
    while(true)
    {
        uint16_t readyFlag = readReadyFlag();
        if(readyFlag == INVALID_VALUE)
        {
            return false;
        }
        if(readyFlag == READY_FLAG_MASK)
        {
            break;
        }
        sleepMs(4);
    }
    uint8_t writeBuffer[2];
    uint16_t chunkAddress = PIXEL_ADDR_START;
    //read from START BY chunks
    for(size_t chunkIndex = 0; chunkIndex < CHUNK_COUNT; ++chunkIndex)
    {
        uint8_t chunk[PIXEL_READ_CHUNK_BYTES] = { 0 };
        writeBuffer[0] = (chunkAddress + (chunkIndex*PIXEL_READ_CHUNK_BYTES)) >> 8 & 0xFF;
        writeBuffer[1] = (chunkAddress + (chunkIndex*PIXEL_READ_CHUNK_BYTES)) & 0xFF;
        i2c.RAWwriteNreadM(writeBuffer, 2, chunk, PIXEL_READ_CHUNK_BYTES);
        for(size_t i = 0; i < PIXEL_READ_CHUNK_BYTES-1; i+=2)
        {
            size_t pixelIndex = chunkIndex * (PIXEL_READ_CHUNK_BYTES / 2) + (i / 2);
            pixelBuffer[pixelIndex] =(static_cast<uint16_t>(chunk[i]) << 8) | chunk[i + 1];
        }
    }
    return true;
}

void Camera::printBufferRaw() const
{
    std::cout << "\033[H\033[J"; // move cursor home + clear screen

    for(size_t row = 0; row < height; ++row) 
    {
        for(size_t column = 0; column < width; ++column) 
        {
            std::cout << std::setw(3) << unsigned(pixelBuffer[width*row + column]) << ' ';
        }
        std::cout << '\n';
    }

    std::cout.flush();
}

void Camera::printBufferCelsius() const
{
    std::cout << "\033[H\033[J"; // move cursor home + clear screen

    for(size_t row = 0; row < height; ++row) 
    {
        for(size_t column = 0; column < width; ++column) 
        {
            int16_t raw = static_cast<int16_t>(pixelBuffer[width*row + column]);
            float celsius = raw / 50.0f;
            std::cout << std::setw(4) << std::fixed << std::setprecision(1)<< celsius << ' ';
        }
        std::cout << '\n';
    }

    std::cout.flush();
}