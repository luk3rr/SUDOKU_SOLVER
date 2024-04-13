/*
 * Filename: solver.h
 * Created on: April 12, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include <cstdint>

#include "constants.h"
#include "vector.h"

/**
 * @brief Class that represents the solver of the sudoku puzzle
 */
class Solver
{
    private:
        uint8_t   m_grid[GRID_SIZE][GRID_SIZE];
        Algorithm m_algorithm;

    public:
        Solver(uint8_t grid[GRID_SIZE][GRID_SIZE], Algorithm algorithm);

        ~Solver();

        void PrintGrid();
};

#endif // SOLVER_H_
