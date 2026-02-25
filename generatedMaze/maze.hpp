#include <cstddef>
#include <iostream>
#include <algorithm>

#include "tile.hpp"

class Maze
{
    private:  
        size_t      width;
        size_t      height;

        uint8_t     startX;
        uint8_t     startY;

        uint8_t     finishX;
        uint8_t     finishY;

        Tile_ds*    tiles;

        size_t index(const int x, const int y) const { return x + width * y; }
        Tile_ds* getNeighbour(const uint8_t, const uint8_t, const uint8_t) const;
        uint8_t getRandomDirection() const;
        void setRandomStart();
        void setRandomFinish();

    public:
        Maze(const size_t, const size_t);
        ~Maze();

        void generateMaze();

        friend std::ostream& operator<<(std::ostream& os, Maze const& maze);
};