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

int main(int argc, char* argv[])
{
    uint16_t grid[GRID_SIZE][GRID_SIZE];

    if (argc != GRID_SIZE + 2)
    {
        std::cerr << "Uso: " << argv[0] << " <entrada>" << std::endl;

        for (int i = 0; i < argc; i++)
        {
            std::cerr << "argv[" << i << "] = " << argv[i] << std::endl;
        }

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
