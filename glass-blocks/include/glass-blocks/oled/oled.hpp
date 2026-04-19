#include <cstdint>
#include <string>
#include <iostream>
#include "glass-blocks/i2c/i2c.hpp"
#include "glass-blocks/oled/font.hpp"
#include "glass-blocks/oled/SSD1306_commands.hpp"

class OLED
{
private:
    I2C         i2c;
    Font        font;

    uint8_t     height;
    uint8_t     width;
    uint8_t*    pixelBuffer;

    bool SSD1306_writeCommand(const uint8_t command);
    bool validateCoords(const uint8_t x,const  uint8_t y) const;
public:
    OLED(const uint8_t width, const uint8_t height, const uint8_t address, const uint8_t adapterNr);
    ~OLED();
    bool init();
    bool render();
    void clearAll();
    bool putPixel(const uint8_t x,const  uint8_t y,const  uint8_t color);
    bool putColumn(const uint8_t x, const uint8_t y, const uint8_t data, const uint8_t color);
    bool drawLine(const uint8_t fromX, const uint8_t fromY, const uint8_t toX, const uint8_t toY, const uint8_t color);
    bool putString(const uint8_t row,const  uint8_t column, const std::string text, const uint8_t color);
};