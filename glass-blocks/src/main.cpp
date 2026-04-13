#include <iostream>

#include "glass-blocks/oled/oled.hpp"

using namespace std;

int main(void)
{
    std::cout << "Glass Blocks Start" << std::endl;
    OLED oled(128,64,0x3c,1);
    bool retVal = oled.init();
    cout << boolalpha <<"OLED init: " << retVal << endl;

    retVal = oled.putPixel(10,10, SSD1306_INVERSE);
    cout << boolalpha << "OLED putPixel: " << retVal << endl;
    retVal = oled.render();
    cout << boolalpha << "OLED render: " << retVal << endl;

    retVal = oled.putPixel(0,0, SSD1306_INVERSE);
    cout << boolalpha << "OLED putPixel: " << retVal << endl;
    retVal = oled.render();
    cout << boolalpha << "OLED render: " << retVal << endl;

    return 0;
}