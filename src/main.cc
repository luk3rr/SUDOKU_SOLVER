/*
 * Filename: main.cc
 * Created on: November  5, 2023
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 *
 * Sudoku solver using state-space search algorithm
 */

#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include "constants.h"
#include "solver.h"

void HelpMessage(int argc, char* argv[])
{

    std::cerr << "Invalid input" << std::endl;
    std::cerr << "Input received: ";

    for (int i = 0; i < argc; i++)
    {
        std::cerr << argv[i] << " ";
    }
    std::cerr << std::endl;

    std::cerr << "Expected input: " << argv[0] << " <algorithm> <grid>" << std::endl;
    std::cerr << "Where <algorithm> is one of the following:" << std::endl;
    std::cerr << "\t- 'B' for Breadth-First Search" << std::endl;
    std::cerr << "\t- 'I' for Iterative Deepening Depth-First Search" << std::endl;
    std::cerr << "\t- 'A' for A* Search" << std::endl;
    std::cerr << "\t- 'U' for Uniform Cost Search" << std::endl;
    std::cerr << "\t- 'G' for Greedy Best-First Search" << std::endl;
    std::cerr << "And <grid> is a " << GRID_SIZE << "x" << GRID_SIZE
              << " matrix representing the Sudoku board" << std::endl;
    std::cerr << "Each cell must be a digit from 0 to " << GRID_SIZE
              << ", where 0 represents an empty cell" << std::endl;
    std::cerr << "Example: " << argv[0]
              << " B 800000000 003600000 070090200 050007000 000045700 000100030 "
                 "001000068 008500010 090000400"
              << std::endl;
}

int main(int argc, char* argv[])
{
    uint16_t grid[GRID_SIZE][GRID_SIZE];

    if (argc != GRID_SIZE + 2)
    {
        HelpMessage(argc, argv);
        return EXIT_FAILURE;
    }

    char algorithm = argv[1][0];

    for (int i = 2; i < argc; i++)
    {
        for (std::size_t j = 0; j < GRID_SIZE; j++)
        {
            grid[i - 2][j] = argv[i][j] - '0';
        }
    }

    sudoku::Solver solver(grid, static_cast<Algorithm>(algorithm));
    solver.Solve();

    return EXIT_SUCCESS;
}
