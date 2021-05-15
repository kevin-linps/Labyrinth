#include "Maze.h"

using namespace labyrinth;

Maze::Maze(int rows, int cols) : rows_(rows), cols_(cols)
{
    for (int i = 0; i < rows_ * cols_; i++)
    {
        structure_.push_back(CellFlag::kNoConnection);
    }

    GenerateMaze();
}

void Maze::PrintToConsole()
{
    for (int i = 0; i < 3 * cols_ + 1; i++)
        std::cout << "_";
    std::cout << '\n';

    int cell;
    for (int y = 0; y < rows_; y++)
    {
        std::string str = "|";
        for (int x = 0; x < cols_; x++)
        {
            cell = structure_.at(GetCellIndex(x, y));
            str += (cell & CellFlag::kConnectsDown) ? "  " : "__";
            str += (cell & CellFlag::kConnectsRight) ? "_" : "|";
        }
        std::cout << str << '\n';
    }
}

void Maze::FindNeighbourCells(int curr, CellFlag dirs[4], int& len)
{
    int x = curr % cols_;
    int y = curr / cols_;
    len = 0;

    if (x - 1 >= 0 && !(structure_.at(GetCellIndex(x - 1, y)) & CellFlag::kIsVisited))
    {
        dirs[len] = CellFlag::kConnectsLeft;
        len++;
    }
    if (y - 1 >= 0 && !(structure_.at(GetCellIndex(x, y - 1)) & CellFlag::kIsVisited))
    {
        dirs[len] = CellFlag::kConnectsUp;
        len++;
    }
    if (x + 1 < cols_ && !(structure_.at(GetCellIndex(x + 1, y)) & CellFlag::kIsVisited))
    {
        dirs[len] = CellFlag::kConnectsRight;
        len++;
    }
    if (y + 1 < rows_ && !(structure_.at(GetCellIndex(x, y + 1)) & CellFlag::kIsVisited))
    {
        dirs[len] = CellFlag::kConnectsDown;
        len++;
    }
}

int Maze::LinkNeighbourCell(int curr_index, CellFlag direction)
{
    int next_index;
    switch (direction)
    {
    case CellFlag::kConnectsLeft:
        next_index = curr_index - 1;
        structure_.at(curr_index) |= CellFlag::kConnectsLeft | CellFlag::kIsVisited;
        structure_.at(next_index) |= CellFlag::kConnectsRight | CellFlag::kIsVisited;
        break;
    case CellFlag::kConnectsRight:
        next_index = curr_index + 1;
        structure_.at(curr_index) |= CellFlag::kConnectsRight | CellFlag::kIsVisited;
        structure_.at(next_index) |= CellFlag::kConnectsLeft | CellFlag::kIsVisited;
        break;
    case CellFlag::kConnectsUp:
        next_index = curr_index - cols_;
        structure_.at(curr_index) |= CellFlag::kConnectsUp | CellFlag::kIsVisited;
        structure_.at(next_index) |= CellFlag::kConnectsDown | CellFlag::kIsVisited;
        break;
    case CellFlag::kConnectsDown:
        next_index = curr_index + cols_;
        structure_.at(curr_index) |= CellFlag::kConnectsDown | CellFlag::kIsVisited;
        structure_.at(next_index) |= CellFlag::kConnectsUp | CellFlag::kIsVisited;
        break;
    default:
        next_index = -1;  // Should never happen
        break;
    }

    return next_index;
}

int Maze::GetCellIndex(int x, int y) { return x + y * cols_; }

void Maze::GenerateMaze()
{
    int start = rand() % structure_.size();

    std::stack<int> path;
    path.push(start);
    structure_.at(start) |= CellFlag::kIsVisited;

    int curr_index, next_index;
    CellFlag dirs[4]; int len, rand_index;

    while (path.size() > 0)
    {
        curr_index = path.top();
        FindNeighbourCells(curr_index, dirs, len);

        if (len == 0)
        {
            path.pop();
        }
        else
        {
            rand_index = rand() % len;
            CellFlag link_direction = dirs[rand_index];
            next_index = LinkNeighbourCell(curr_index, link_direction);
            path.push(next_index);
        }
    }
}