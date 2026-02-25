#include <iostream>

#include "maze.hpp"

using namespace std;

int main(void)
{
    Maze maze(10,10);
    cout << maze << endl;
    maze.generateMaze();
    cout << maze << endl;

    return 0;
}