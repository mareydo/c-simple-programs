#include <stack>
#include <random>

#include "maze.hpp"

Maze::Maze(const size_t width, const size_t height)
{
    this->width = width;
    this->height = height;

    this->startX = 0;
    this->startY = 0;

    this->tiles = new Tile_ds[ width * height];
    for(size_t y = 0; y < height; ++y)
    {
        for(size_t x = 0; x < width; ++x)
        {
            tiles[index(x, y)].x = x;
            tiles[index(x, y)].y = y;
            tiles[index(x, y)].walls = 0;
            tiles[index(x, y)].walls = TOP | RIGHT | BOTTOM | LEFT;
            tiles[index(x, y)].visited = false;
        }
    }
}

Maze::~Maze()
{
    delete[] this->tiles;
}

Tile_ds* Maze::getNeighbour(const uint8_t direction, const uint8_t x, const uint8_t y) const
{
    switch(direction)
    {
        case TOP:
            {
                if(y == 0) return nullptr;
                return &tiles[index(x,y-1)];
            }
        case RIGHT:
            {
                if(x == width -1) return nullptr;
                return &tiles[index(x+1,y)];
            }
        case BOTTOM:
            {
                if(y == height -1) return nullptr;
                return &tiles[index(x,y+1)];
            }
        case LEFT:
            {
                if(x == 0) return nullptr;
                return &tiles[index(x-1,y)];
            }
        default:
            return nullptr;
    }
}
uint8_t Maze::getRandomDirection() const
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0,3);
    int randomValue = distrib(gen);
    return randomValue;
}

void Maze::setRandomStart()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distribX(0, width/2);
    std::uniform_int_distribution<> distribY(0, height/2);

    this->startX = distribX(gen);
    this->startY = distribY(gen);

    tiles[index(this->startX, this->startY)].isStart = true;
}

void Maze::setRandomFinish()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distribX(width/2, width-1);
    std::uniform_int_distribution<> distribY(height/2, height-1 );

    this->finishX = distribX(gen);
    this->finishY = distribY(gen);
    
    tiles[index(this->finishX, this->finishY)].isFinish = true;
}

void Maze::generateMaze()
{
    setRandomStart();
    setRandomFinish();


    Tile_ds* startTile = &tiles[index(startX, startY)];
    std::stack<Tile_ds*> visited;
    visited.push(startTile);
    while(!visited.empty())
    {
        Tile_ds* actual = visited.top();
        actual->visited = true;

        Tile_ds* neighbour = nullptr;
        bool directionTested[4] = {false, false, false, false};
        uint8_t direction;
        while(true)
        {
            // dead end
            if(std::all_of(directionTested, directionTested+4, [](bool n) { return n; }))
            {
                neighbour = nullptr;
                visited.pop();
                break;
            }

            direction = getRandomDirection();
            //optimalization
            if(directionTested[direction])
            {
                continue;
            }
            directionTested[direction] = true;

            neighbour = getNeighbour(1<<direction, actual->x, actual->y);
            //way to go
            if(neighbour != nullptr && !neighbour->visited)
            {
                break;
            }

        }
        //move to a new cell
        if(neighbour != nullptr && !neighbour->visited)
        {
            visited.push(neighbour);
            actual->removeWall(1<<direction);
            neighbour->removeOppositeWall(1<<direction);
        }
    }
}



std::ostream& operator<<(std::ostream& os, Maze const& maze)
{
    os << "Start X: " << unsigned(maze.startX) << ", Start Y: " << unsigned(maze.startY) << '\n';
    os << "Finish X: " << unsigned(maze.finishX) << ", Finish Y: " << unsigned(maze.finishY) << '\n';
    for(size_t row = 0; row < maze.height; ++row)
    {
        for(size_t column = 0; column < maze.width; ++column)
        {
            os << maze.tiles[maze.index(column, row)].printTop();
        }
        os << '\n';
        for(size_t column = 0; column < maze.width; ++column)
        {
            os << maze.tiles[maze.index(column, row)].printMiddle();
        }
        os << '\n';
        for(size_t column = 0; column < maze.width; ++column)
        {
            os << maze.tiles[maze.index(column, row)].printBottom();
        }
        os << '\n';
    }
    return os;
}