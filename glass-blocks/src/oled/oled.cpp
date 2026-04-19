#include <cmath>
#include "glass-blocks/oled/oled.hpp"

OLED::OLED(const uint8_t width, 
    const uint8_t height, 
    const uint8_t address, 
    const uint8_t adapterNr) : i2c(address, adapterNr), font()
{
    this->width = width;
    this->height = height;
    this->pixelBuffer = new uint8_t[width * ((height+7)/8)]; //rounding height to multiples of 8
    this->clearAll();
}

OLED::~OLED()
{
    delete[] pixelBuffer;
}

bool OLED::SSD1306_writeCommand(const uint8_t command)
{
    uint8_t buffer[2] = {0x00, command};
    return i2c.writeBytes(buffer, 2);
    
}

bool OLED::init()
{
    std::cout << "OLED init" << std::endl;
    if(!i2c.init())
    {
        return false;
    }
    
    SSD1306_writeCommand( SSD1306_DISPLAY_OFF);
	SSD1306_writeCommand( SSD1306_SET_DISPLAY_CLOCK_DIV);
	SSD1306_writeCommand( 0x80);
	SSD1306_writeCommand( SSD1306_SET_MULTIPLEX );
	SSD1306_writeCommand( this->height -1 );
	SSD1306_writeCommand( SSD1306_SET_DISPLAY_OFFSET );
	SSD1306_writeCommand( 0x00 );
	SSD1306_writeCommand( SSD1306_SET_START_LINE);
	SSD1306_writeCommand( SSD1306_CHARGE_PUMP );
	SSD1306_writeCommand( 0x14 ); //0x14
	SSD1306_writeCommand( SSD1306_MEMORY_MODE );
	SSD1306_writeCommand( 0x00 );  //Horizontal Addressing Mode is Used
	SSD1306_writeCommand( SSD1306_SEG_REMAP| 0x01);
	SSD1306_writeCommand( SSD1306_COM_SCAN_DEC );

    uint8_t comPins = 0;
    uint8_t contrastControl = 0;
    switch (height)
    {
        case 64:
        {
            comPins = 0x12 ;
            contrastControl = 0xCF;
            break;
        }
        default:
            std::cerr << "unsupported height: " << unsigned(height) << std::endl;
            return false;
            break;
    }
    SSD1306_writeCommand(SSD1306_SET_COM_PINS);
    SSD1306_writeCommand(comPins);
    SSD1306_writeCommand(SSD1306_SET_CONTRAST);
    SSD1306_writeCommand(contrastControl);
    SSD1306_writeCommand( SSD1306_SET_PRECHARGE );
	SSD1306_writeCommand( 0xF1 ); //0xf1
	SSD1306_writeCommand( SSD1306_SET_VCOM_DETECT );
	SSD1306_writeCommand( 0x40 );
	SSD1306_writeCommand( SSD1306_DISPLAY_ALL_ON_RESUME );
	SSD1306_writeCommand( SSD1306_NORMAL_DISPLAY );
	SSD1306_writeCommand( SSD1306_DEACTIVATE_SCROLL );
	SSD1306_writeCommand( SSD1306_DISPLAY_ON );

       std::cout << "OLED inited" << std::endl;
    return true;
}

bool OLED::render()
{
    SSD1306_writeCommand( SSD1306_PAGE_ADDR );
    SSD1306_writeCommand( 0x00 );
    SSD1306_writeCommand( 0x07 );

    SSD1306_writeCommand( SSD1306_COLUMN_ADDR );
    SSD1306_writeCommand( 0x00);
    SSD1306_writeCommand( width -1);

    for (size_t i = 0; i < width * ((height + 7) / 8); i += 16)
    {
        uint8_t buffer[17];
        buffer[0] = 0x40;

        for (size_t j = 0; j < 16; ++j)
        {
            buffer[j + 1] = pixelBuffer[i + j];
        }

        if (!i2c.writeBytes(buffer, 17))
            return false;
    }
    return true;
}

void OLED::clearAll()
{
    for(size_t i = 0; i < width * ((height+7)/8); ++i) { pixelBuffer[i] = 0; }
}

bool OLED::validateCoords(const uint8_t x,const  uint8_t y) const 
{
    /*if(x < 0)
    {
        std::cerr << "X Out Of Bounds: " << unsigned(x) << " < 0" << std::endl;
        return false;
    }*/
    if(x >= width)
    {
        std::cerr << "X Out Of Bounds: " << unsigned(x) << " > " << unsigned(width) << std::endl;
        return false;
    }
    /*if(y < 0)
    {
        std::cerr << "Y Out Of Bounds: " << unsigned(y) << " < 0" << std::endl;
        return false;
    }*/
    if(y >= height)
    {
        std::cerr << "Y Out Of Bounds: " << unsigned(y) << " > " << unsigned(height) << std::endl;
        return false;
    }
    return true;
}

bool OLED::putPixel(const uint8_t x,const  uint8_t y,const uint8_t color)
{
    if(!validateCoords(x,y)) return false;

    switch (color)
    {
        case SSD1306_WHITE:
            pixelBuffer[x + (y / 8) * width] |= (1 << (y & 7));
            break;
        case SSD1306_BLACK:
            pixelBuffer[x + (y / 8) * width] &= ~(1 << (y & 7));
            break;
        case SSD1306_INVERSE:
            pixelBuffer[x + (y / 8) * width] ^= (1 << (y & 7));
            break;
        default:
            return false;
    }
    return true;
}

bool OLED::putColumn(const uint8_t x, const uint8_t y, const uint8_t data, const uint8_t color)
{
    if(!validateCoords(x,y)) return false;

    switch (color)
    {
        case SSD1306_WHITE:
            pixelBuffer[x + (y / 8) * width] |= data;
            break;
        case SSD1306_BLACK:
            pixelBuffer[x + (y / 8) * width] &= ~(data);
            break;
        case SSD1306_INVERSE:
            pixelBuffer[x + (y / 8) * width] ^= data;
            break;
        default:
            return false;
    }
    return true;
}

bool OLED::drawLine(const uint8_t fromX, const uint8_t fromY, const uint8_t toX, const uint8_t toY, const uint8_t color)
{
    if(!validateCoords(fromX,fromY)) return false;
    if(!validateCoords(toX,toY)) return false;

    uint8_t x1,x2,y1,y2,dx,dy;
    uint8_t x,y;

    if(fromX == toX)
    {
        uint8_t ya = fromY < toY ? fromY : toY;
        uint8_t yb = fromY > toY ? fromY : toY;
        for (uint8_t y = ya; y <= yb; ++y) 
        {
            if (!putPixel(fromX, y, color)) return false;
        }
    }
    else if(fromX > toX)
    {
        x2 = fromX;
        y2 = fromY;
        x1 = toX;
        y1 = toY;
    }
    else
    {
        x2 = toX;
        y2 = toY;
        x1 = fromX;
        y1 = fromY;
    }

    double m = (double)(y2 - y1)/(double)(x2 - x1);

    for(x = x1; x <= x2; ++x)
    {
        y = m * ( x - x1 ) + y1;
        if(!putPixel(x,y,color)) return false;
    }

    return true;
}
bool OLED::putString(const uint8_t x,const uint8_t y, const std::string text, const uint8_t color)
{
    if(!validateCoords(x,y)) return false;

    for(size_t letterIndex = 0; letterIndex < text.length(); ++letterIndex)
    {
        const uint8_t* fontData = font.getChar(text[letterIndex]);
        for(size_t letterColumn = 0; letterColumn < FONT_WIDTH; ++letterColumn)
        {
            if(!putColumn((FONT_WIDTH+1)*letterIndex + x + letterColumn, 
                            y,
                            *(fontData + letterColumn), 
                            color))
            {
                return false;
            }
        }
        if(!putColumn(FONT_WIDTH+1*letterIndex + x + 8, 
                            y,
                            0x00, 
                            SSD1306_BLACK))
            {
                return false;
            }
    }
    return true;
}