#include <cstdint>
#include <string>

#define WHITE_COLOR 0
#define BLACK_COLOR 1

class OLED : public I2C
{
private:
    uint8_t height;
    uint8_t width;
    uint8_t* pixelBuffer;

    bool SSD1306_writeCommand(const uint8_t command);
    bool validateCoords(const uint8_t x,const  uint8_t y) const;
public:
    OLED(const uint_8 width, const uint_8 height, const uint_8 address);
    ~OLED();
    bool init();
    bool render();
    bool clearAll();
    bool putPixel(const uint8_t x,const  uint8_t y,const  uint8_t color);
    bool drawLine(const uint8_t fromX, const uint8_t fromY, const uint8_t toX, const uint8_t toY);
    bool putString(const uint8_t row,const  uint8_t column, const string text);
};