#include "glass-blocks/oled/oled.hpp"
#include "glass-blocks/oled/SSD1306_commands.hpp"

OLED::OLED(const uint_8 width, const uint_8 height, const uint_8 address);
{
    this->width = width;
    this->height = height;
    this->address = address;
    this->pixelBuffer = new uint8_t[width * ((height+7)/8)]; //rounding height to multiples of 8
}

OLED::~OLED()
{
    delete[] pixelBuffer;
}

bool OLED::SSD1306_writeCommand(const uint8_t command)
{
    //start transmission
    writeByte(0x00);
    writeByte(command);
    //end transmission
}

bool OLED::init()
{
    //init I2C
    SSD1306_writeCommand( SSD1306_DISPLAY_OFF);
	SSD1306_writeCommand( SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO);
	SSD1306_writeCommand( 0x80);
	SSD1306_writeCommand( SSD1306_SET_MULTIPLEX_RATIO );
	SSD1306_writeCommand( this->height -1 );
	SSD1306_writeCommand( SSD1306_SET_DISPLAY_OFFSET );
	SSD1306_writeCommand( 0x00 );
	SSD1306_writeCommand( SSD1306_SET_START_LINE);
	SSD1306_writeCommand( SSD1306_CHARGE_PUMP );
	SSD1306_writeCommand( 0x10 ); //0x14
	SSD1306_writeCommand( SSD1306_MEMORY_ADDR_MODE );
	SSD1306_writeCommand( 0x00 );  //Horizontal Addressing Mode is Used
	SSD1306_writeCommand( SSD1306_SET_SEGMENT_REMAP| 0x01);
	SSD1306_writeCommand( SSD1306_COMSCANDEC );

    uint8_t comPins = 0;
    uint8_t contrastControl = 0;
    switch (height)
    {
        case 64:
        {
            comPins = 0x12 ;
            contrastControl = 0x9F; //0xCF
            break;
        }
        default:
            std::cerr << "unsuported height: " << unsigned(height) << std::endl;
            return false;
            break;
    }
    SSD1306_writeCommand(SSD1306_SETCOMPINS);
    SSD1306_writeCommand(comPins);
    SSD1306_writeCommand(SSD1306_SETCONTRAST);
    SSD1306_writeCommand(contrast);

    SSD1306_writeCommand( SSD1306_SET_PRECHARGE_PERIOD );
	SSD1306_writeCommand( 0xF1 ); //0x22
	SSD1306_writeCommand( SSD1306_SETVCOMDETECT );
	SSD1306_writeCommand( 0x40 );
	SSD1306_writeCommand( SSD1306_DISPLAY_ALL_ON_RESUME );
	SSD1306_writeCommand( SSD1306_NORMAL_DISPLAY );
	SSD1306_writeCommand( SSD1306_DEACTIVATE_SCROLL );
	SSD1306_writeCommand( SSD1306_DISPLAY_ON );

    return true;
}

bool OLED::render()
{
    SSD1306_writeCommand( SSD1306_PAGEADDR );
    SSD1306_writeCommand( 0x00 );
    SSD1306_writeCommand( 0xFF );
    SSD1306_writeCommand( SSD1306_COLUMNADDR );

    SSD1306_writeCommand( 0x00);
    SSD1306_writeCommand( width -1);

    uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
    uint8_t *ptr = pixelBuffer;
    if (wire) 
    {
        //TRANSACTION_START
        writeByte((uint8_t)0x40);
        uint16_t bytesOut = 1;
        while (count--) {
        // ?
        /*if (bytesOut >= WIRE_MAX) {
            //TRANSACTION_END
            //TRANSACTION_START
            writeByte((uint8_t)0x40);
            bytesOut = 1;
        }*/
        writeByte(*ptr++);
        bytesOut++;
        }
        wire->endTransmission();
    }
    //TRANSACTION_END






}

void OLED::clearAll()
{
    for(size_t i = 0; i < width * ((height+7)/8); ++i) { pixelBuffer[i] = 0; }
}

bool validateCoords(const uint8_t x,const  uint8_t y) const 
{
    if(x < 0)
    {
        std::cerr << "X Out Of Bounds: " << unsigned(x) << " < 0" << std::endl;
        return false;
    }
    if(x > width)
    {
        std::cerr << "X Out Of Bounds: " << unsigned(x) << " > " << unsigned(width) << std::endl;
        return false;
    }
    if(y < 0)
    {
        std::cerr << "Y Out Of Bounds: " << unsigned(y) << " < 0" << std::endl;
        return false;
    }
    if(y > height)
    {
        std::cerr << "Y Out Of Bounds: " << unsigned(y) << " > " << unsigned(height) << std::endl;
        return false;
    }
    return true;
}

bool OLED::putPixel(const uint8_t x,const  uint8_t y,const  uint8_t color)
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
    }
    return false;
}

bool OLED::drawLine(const uint8_t fromX, const uint8_t fromY, const uint8_t toX, const uint8_t toY)
{

}
bool OLED::putString(const uint8_t row,const  uint8_t column, const string text)
{

}