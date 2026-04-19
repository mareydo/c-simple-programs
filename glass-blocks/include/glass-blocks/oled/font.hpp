// font copied from
// https://github.com/BaronWilliams/Vertical-Fonts/blob/master/font7x8.c
#include <cstdint>

#define FONT_CHARS 95
#define FONT_WIDTH 7

class Font
{
private:
    uint8_t fontData[FONT_CHARS][FONT_WIDTH];
public:
    Font();
    const uint8_t* getChar(const char c) const;
};