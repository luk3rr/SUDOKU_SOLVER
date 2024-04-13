/*
 * Filename: solver.cc
 * Created on: April 12, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include "solver.h"
#include "constants.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <pthread.h>

Solver::Solver(uint8_t grid[GRID_SIZE][GRID_SIZE], Algorithm algorithm)
{
    this->m_algorithm = algorithm;
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            this->m_grid[i][j] = grid[i][j];
        }
    }
}

Solver::~Solver() { }

void Solver::PrintGrid()
{
    std::cout << "Algorithm: ";
    // Print letters according to the algorithm
    switch (m_algorithm)
    {
        case Algorithm::BFS:
            std::cout << "BFS" << std::endl;
            break;
        case Algorithm::IDDFS:
            std::cout << "IDDFS" << std::endl;
            break;
        case Algorithm::UCS:
            std::cout << "UCS" << std::endl;
            break;
        case Algorithm::A_STAR:
            std::cout << "A*" << std::endl;
            break;
        case Algorithm::GBFS:
            std::cout << "GREEDY" << std::endl;
            break;
        default:
            std::cout << "UNKNOWN" << std::endl;
            break;
    }

    // Print the grid
    for (std::size_t i = 0; i < GRID_SIZE; i++)
    {
        if (i % SUBGRID_SIZE == 0 and i != 0)
        {
            std::cout << "------+-------+------" << std::endl;
        }
        for (std::size_t j = 0; j < GRID_SIZE; j++)
        {
            if (j % SUBGRID_SIZE == 0 and j != 0)
            {
                std::cout << "| ";
            }
            std::cout << (int)m_grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
