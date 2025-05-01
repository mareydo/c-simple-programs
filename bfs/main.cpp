#include <iostream>
#include <cstdint>

/**
 * @brief 
 * program simulates a labyrinth with "levers" that flip walls (i.e., change walls to paths and vice versa) based on a vertical vector. 
   It then tries every combination of lever activations (using bitmasks) to find the shortest path from the top-left corner [1;1] to a goblet 
   located somewhere in the maze.
 */

// ! this program needs more debug to properly work with the vector, the bfs is fine 
using namespace std;

enum TileState { NOT_FOUND, OPEN, CLOSED };
struct Tile
{
    bool isWall = false;
    bool originalWall = false;
    bool isGoblet = false;
    TileState state = NOT_FOUND;
    uint32_t distance = 0;
    uint32_t x=0;
    uint32_t y=0;  
    Tile* pred=nullptr;
    inline void print() const {cout << '[' << x+1 << ';' << y+1 << ']';}
};

class Queue
{    
private:
    Tile** data;
    size_t size;
    uint32_t first;
    uint32_t last;
public:
    Queue(const size_t& size): size(size)
    {
        data = new Tile*[size];
        first = last = 0;
    }
    ~Queue()
    {
        delete [] data;
    }
    void push(Tile* tile)
    {
        data[last] = tile;
        ++last;
    }
    Tile* pop()
    {
        ++first;
        return data[first-1];
    }
    Tile* popLast() 
    { 
        if(last==0) return nullptr;
        --last;
        return data[last]; 
    }
    inline bool isEmpty() const { return first == last; }
};

class Lever
{
private:
    int32_t yPos;
    size_t size;
    bool* changeVector = nullptr;

public:
    Lever(const size_t& size, const int32_t& yPos): yPos(yPos), size(size)
    {
        changeVector = new bool[size];
    }
    ~Lever()
    {
        delete [] changeVector;
    }
    inline int32_t getYpos() const { return yPos; }
    inline void setYpos(const int32_t& yPos) { this->yPos = yPos; }
    inline void setValue(const size_t& index, const bool& value) { this->changeVector[index] = value; }
    inline bool getValue(const size_t& index) const { return changeVector[index]; }
};

class Labyrinth
{
private:
    size_t sideLength;
    uint32_t minimalDistance = 0;
    Tile** tiles = nullptr;
    Tile* goblet= nullptr;

public:
    Labyrinth(const size_t& side): sideLength(side)
    {
        tiles = new Tile*[sideLength];
        for(size_t i=0; i < sideLength; ++ i)
        {
            tiles[i] = new Tile[sideLength];
        }
    }
    ~Labyrinth()
    {
        for(size_t i = 0; i < sideLength; ++ i)
        {
            delete [] tiles[i];
        }
        delete [] tiles;
    }
    void setGobletPoint(const uint32_t& x, const uint32_t& y)
    {
        goblet = &tiles[x][y];
        goblet->isGoblet = true;
        minimalDistance = x + y;
    }
    inline bool isMinimalDistance(const int&dst) const{ return dst <= (int)minimalDistance; }
    
    void printPath(const int& length) const
    {
        Queue path(length);
        Tile* actTile = goblet;
        while(1)
        {
            path.push(actTile);
            actTile = actTile->pred;
            if(actTile==nullptr) break;
        }
        while(1)
        {
            actTile = path.popLast();
            actTile->print();
            if(!path.isEmpty()) {cout << ',';}
            else break;
        }
        cout << endl;
    }

    //DEBUG
    void print() const
    {
        for(int y = sideLength-1; y >=0 ; --y) 
        {
            for (size_t x = 0; x < sideLength ; ++x) 
            {
                if(this->tiles[x][y].isWall)
                    cout << 'x';
                else if (this->tiles[x][y].isGoblet)
                    cout << 'G';
                else
                    cout << '_';
            }
            cout << endl;
        }
    }
    void setTile(const uint32_t& x, const uint32_t& y, const bool& isWall)
    {
        Tile* tile = &this->tiles[x][y];
        tile->isWall = tile->originalWall =isWall;
        tile->x = x;
        tile->y = y;
    }
    void applyLever(const Lever* lever)
    {
        size_t x,y;
        bool leverValue;
        for(y = 0; y < sideLength; ++y)
        {
            for(x = 0; x < sideLength; ++x)
            {
                leverValue = lever->getValue(x);
                tiles[x][y].isWall = (tiles[x][y].isWall != leverValue);
            }
        }
    }
    void reset()
    {
        size_t x,y;
        for(y = 0; y < sideLength; ++y)
        {
            for(x = 0; x < sideLength; ++x)
            {
                tiles[x][y].isWall = tiles[x][y].originalWall;
            }
        }
    }

    int BFS()
    {
        if(tiles[0][0].isWall) return -1;
        //SET ALL v not found
        //d=p=null
        size_t x,y;
        Tile* actTile = nullptr;
        Tile* u = nullptr;
        for(y = 0; y < sideLength; ++y)
        {
            for(x = 0; x < sideLength; ++x)
            {
                actTile = &tiles[x][y];
                actTile->state = NOT_FOUND;
                actTile->distance = 0;
                actTile->pred = nullptr;
            }
        }
        //q is queue
        Queue Q(sideLength*sideLength);
        //q.push(v)
        Q.push(&this->tiles[0][0]);
        //d(s)=0
        //while q
        while(! Q.isEmpty())
        {
            //s = q.pop
            actTile = Q.pop();
            if(actTile->isGoblet)
            {
                return actTile->distance;
            }
            //for all neighbours
            if( actTile->y < sideLength -1 )
            {
                u = &tiles[actTile->x][actTile->y+1];
                if( (u->state == NOT_FOUND)&&(!u->isWall) )
                {
                    u->state = OPEN;
                    u->distance = actTile->distance+1;
                    u->pred = actTile;
                    Q.push(u);
                }
            }
            if( actTile->x < sideLength -1 )
            {
                u = &tiles[actTile->x+1][actTile->y];
                if( (u->state == NOT_FOUND)&&(!u->isWall) )
                {
                    u->state = OPEN;
                    u->distance = actTile->distance+1;
                    u->pred = actTile;
                    Q.push(u);
                }
            }
            if( actTile->y > 0 )
            {
                u = &tiles[actTile->x][actTile->y-1];
                if( (u->state == NOT_FOUND)&&(!u->isWall) )
                {
                    u->state = OPEN;
                    u->distance = actTile->distance+1;
                    u->pred = actTile;
                    Q.push(u);
                }
            }
            if( actTile->x > 0 )
            {
                u = &tiles[actTile->x-1][actTile->y];
                if( (u->state == NOT_FOUND)&&(!u->isWall) )
                {
                    u->state = OPEN;
                    u->distance = actTile->distance+1;
                    u->pred = actTile;
                    Q.push(u);
                }
            }
            //close s
            actTile->state = CLOSED;
        }
        //not found
        return -1;
    }
};

void bubbleSort(Lever** levers, const size_t& leverCount) 
{ 
    if(leverCount == 0) return;
    size_t i, j; 
    for (i = 0; i < leverCount -1 ; i++)     
    {
        for (j = 0; j < leverCount-i-1; j++) 
        {
            if (levers[j]->getYpos() > levers[j+1]->getYpos())
            {
                    Lever* temp = levers[j]; 
                    levers[j] = levers[i]; 
                    levers[i] = temp;
            }             
        }  
    }   
} 


int main(void)
{
    //*************************************//
    size_t side;
    uint16_t leverCount = 0; 
    cin >> side;
    cin >> leverCount;
    //*************************************//
    Labyrinth l(side);
    Lever** levers = new Lever*[leverCount];
    Lever** leversOrdered = new Lever*[leverCount];
    //*************************************//
    //INIT LEVERS
    if(leverCount)
    {
        for(size_t i = 0; i < leverCount; ++i )
        {
            int32_t leverY=0;
            char inp = 'x';
            cin >> leverY;
            
            Lever* newLever = new Lever(side, leverY);
            //INIT LEVER VECTOR
            for(size_t j =0; j < side; ++j)
            {
                cin >> inp;
                newLever->setValue(j, inp == '1');
            }
            levers[i] = newLever;
            leversOrdered[i] = newLever;
        }
        bubbleSort(leversOrdered, leverCount);
    }
    //*************************************//
    for(size_t y = 0; y < side ; ++y)
    {
        for(size_t x = 0; x < side ; ++x)
        {
            char inp = 'x';
            cin >> inp;
            if(inp == '1')
            {
                l.setTile(x,y, true);
            }
            else if(inp == '0')
            {
                l.setTile(x,y, false);
            }
            else
            {
                --x;
            }
        }
    }
    uint32_t gobletX, gobletY;
    cin >> gobletX;
    cin >> gobletY;
    l.setGobletPoint(gobletX-1, gobletY-1);  
        

    uint16_t leverChangeVector=0; 
    uint16_t minimalLeverChangeVector=0;  
    uint16_t maxLeverBit = (1 << leverCount);
    
    int BFSRes = -1;
    bool wayFound = false;


    int minBFSRes = INT32_MAX;
    int leverPath = INT32_MAX;
    int negLeverPath = 0;

    //l.print();

    for(leverChangeVector = 0; leverChangeVector < maxLeverBit  ; ++leverChangeVector)
    {
        leverPath = INT32_MAX;
        negLeverPath = 0;

        if(leverCount)
        {
            for(int16_t i = 0; i<leverCount; ++i)
            {
                if(leverChangeVector & (1 << i))
                {                     
                    if( (negLeverPath==0) && (leversOrdered[i]->getYpos() == -1) )
                    {
                        negLeverPath = 1;
                    }
                    else if(leverPath == INT32_MAX)
                    {
                        leverPath = leversOrdered[i]->getYpos();                    
                    }                    
                    l.applyLever(leversOrdered[i]);
                }
            }
        }
        BFSRes = l.BFS();
        if(BFSRes != -1)
        {
            wayFound = true;
            if (leverPath == INT32_MAX) leverPath = 0;
            leverPath = (leverPath+negLeverPath)*2;
            if(BFSRes + leverPath <= minBFSRes) 
            {
                minBFSRes = BFSRes + leverPath;
                minimalLeverChangeVector = leverChangeVector;
            }
            //found smallest possible result
            if(l.isMinimalDistance(BFSRes)) break;
        }        
        l.reset();
    }
    l.reset();
    if(wayFound)
    {
        cout << minBFSRes << endl;
        if(leverCount)
        {
            for(uint8_t i = 0; i < leverCount; ++i)
            {
                if(minimalLeverChangeVector & 1 << i)
                {
                    l.applyLever(leversOrdered[i]);                    
                }                
            } 
            for(uint8_t i = 0; i < leverCount; ++i)
            {
                if(minimalLeverChangeVector & 1 << i)
                {
                    cout<< 1;
                }
                else
                {
                    cout << 0;
                }
                
            }
            cout << endl;
        }
        BFSRes = l.BFS();
        l.printPath(BFSRes+1);
        //l.print();
    }
    else
    {
        cout << -1 << endl;
    }
    
    for(size_t i = 0; i < leverCount; ++i )
    {
        delete levers[i];    
    }
    delete [] levers;
    delete [] leversOrdered;
    return 0;
}