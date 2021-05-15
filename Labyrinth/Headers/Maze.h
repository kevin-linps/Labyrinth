#ifndef LABYRINTH_MAZE_H_
#define LABYRINTH_MAZE_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stack>

namespace labyrinth
{
    enum CellFlag
    {
        kConnectsLeft = 1 << 0,
        kConnectsRight = 1 << 1,
        kConnectsUp = 1 << 2,
        kConnectsDown = 1 << 3,
        kIsVisited = 1 << 4,
        kNoConnection = 0
    };

    class Maze
    {
    private:

        void FindNeighbourCells(int curr, CellFlag dirs[4], int& len);
        int LinkNeighbourCell(int curr_index, CellFlag direction);
        int GetCellIndex(int x, int y);

        void GenerateMaze();

    public:

        Maze(int rows, int cols);

        int rows_, cols_;
        std::vector<int> structure_;

        void PrintToConsole();
    };
}
#endif