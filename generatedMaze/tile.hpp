#include <cstdint>
#include <string>

#define TOP    0b0001   //1 - 2,0
#define RIGHT  0b0010   //2 - 2,1
#define LEFT   0b0100   //4 - 2,2
#define BOTTOM 0b1000   //8 - 2,3


struct Tile_ds
{
    uint8_t walls;

    uint8_t x;
    uint8_t y;

    bool visited;
    bool isStart;
    bool isFinish;

    void removeWall(uint8_t direction)
    {
        walls &= ~direction;
    }
    void removeOppositeWall(uint8_t direction)
    {
        if(direction & TOP)             removeWall(BOTTOM);
        else if(direction & RIGHT)      removeWall(LEFT);
        else if(direction & LEFT)       removeWall(RIGHT);
        else if(direction & BOTTOM)     removeWall(TOP);
    }

    std::string printTop()
    {
        if(walls & TOP) return "*--*";
        else            return "*  *";
    }
    std::string printMiddle()
    {
        std::string middleString = "";
        if(walls & LEFT)    middleString += "|";
        else                middleString += " ";
        
        if(isStart)         middleString += "SS";
        else if(isFinish)   middleString += "FF";
        else                middleString += "  ";
        
        if(walls & RIGHT)   middleString += "|";
        else                middleString += " ";
        
        return middleString;
    }
    std::string printBottom()
    {
        if(walls & BOTTOM) return "*--*";
        else            return "*  *";
    }

    friend std::ostream& operator<<(std::ostream& os, Tile_ds const& tile)
    {
        os << "X: " << unsigned(tile.x) << ", Y: " << unsigned(tile.y);
        return os;
    }
};