#include <iostream>

#include "glass-blocks/oled/oled.hpp"
#include "glass-blocks/camera/camera.hpp"

using namespace std;

#define I2C_ADDRESS_OLED 0x3c
#define I2C_ADDRESS_CAMERA 0x66

int main(void)
{
    std::cout << "Glass Blocks Start" << std::endl;
    Camera camera(32,24,I2C_ADDRESS_CAMERA,1);

    bool retVal = camera.init();
    cout << boolalpha <<"Camera init: " << retVal << endl;


    Camera::CameraVersion version = camera.readVersion();
    if(version.major == 0)
    {
        cout << "Zero major" << endl;
    }
    else
    {
        cout << "Major: " << unsigned(version.major) << endl;
        cout << "Minor: " << unsigned(version.minor) << endl;
        cout << "Patch: " << unsigned(version.patch) << endl;
    }

    cout << "Mode: " << camera.getMeasuringMode() << endl;

    retVal = camera.setMeasuringMode(CONT_MEASURING_MODE);
    cout << boolalpha << "Camera setmode: " << retVal << endl;
    cout << "Mode: " << camera.getMeasuringMode() << endl;

    retVal = camera.setOutputDataFormat(OUTPUT_DATA_ABSOLUTE);
    cout << boolalpha << "Output Data format set: " << retVal << endl;
    cout << "Output data format: " << camera.getOutputDataFormat() << endl;

    //bool readyFlag = camera.isReadyFlag();
    //cout << boolalpha << "readyFlag: " << readyFlag << endl;
    /*
    for(int i = 0; i < 10000; ++i)
    {
        camera.update();
        camera.printBufferCelsius();
    }
    */
    /*
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

    retVal = oled.putString(0,0, "Hello World!", SSD1306_WHITE);
    cout << boolalpha << "OLED putString: " << retVal << endl;
    retVal = oled.render();
    cout << boolalpha << "OLED render: " << retVal << endl;

    retVal = oled.putString(0,1, "xxxxxxx", SSD1306_WHITE);
    cout << boolalpha << "OLED putString: " << retVal << endl;
    retVal = oled.render();
    cout << boolalpha << "OLED render: " << retVal << endl;

    retVal = oled.putString(0,0, "1", SSD1306_WHITE);
    retVal = oled.putString(0,8, "2", SSD1306_WHITE);
    retVal = oled.putString(0,16, "3", SSD1306_WHITE);
    retVal = oled.putString(0,24, "4", SSD1306_WHITE);
    retVal = oled.putString(0,32, "5", SSD1306_WHITE);
    retVal = oled.putString(0,40, "6", SSD1306_WHITE);
    retVal = oled.putString(0,48, "7", SSD1306_WHITE);
    retVal = oled.putString(0,56, "8", SSD1306_WHITE);
    retVal = oled.putString(110,56, "0", SSD1306_WHITE);
    retVal = oled.render();

    retVal = oled.drawLine(0, 0, 127, 0, SSD1306_WHITE);
    cout << boolalpha << "OLED 1 drawLine: " << retVal << endl;

    retVal = oled.drawLine(0, 63, 127, 63, SSD1306_WHITE);
    cout << boolalpha << "OLED 2 drawLine: " << retVal << endl;

    retVal = oled.drawLine(0, 0, 0, 63, SSD1306_WHITE);
    cout << boolalpha << "OLED 3 drawLine: " << retVal << endl;
    
    retVal = oled.drawLine(127, 0, 127, 63, SSD1306_WHITE);
    cout << boolalpha << "OLED 4 drawLine: " << retVal << endl;


    retVal = oled.drawLine(0, 0, 127, 63, SSD1306_WHITE);
    cout << boolalpha << "OLED 5 drawLine: " << retVal << endl;
    retVal = oled.drawLine(127, 0, 0, 63, SSD1306_WHITE);
    cout << boolalpha << "OLED 6 drawLine: " << retVal << endl;

    retVal = oled.render();
    cout << boolalpha << "OLED render: " << retVal << endl;
    */
    return 0;
}




Channel Breakpoint
{
public:
    void subscribe();




};
