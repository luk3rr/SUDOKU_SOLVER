/*
 * Filename: grid_utils.cc
 * Created on: April 13, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include "grid_utils.h"

namespace grid
{
    bool IsInRow(uint16_t grid[GRID_SIZE][GRID_SIZE], uint16_t row, uint16_t num)
    {
        for (std::size_t col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == num)
            {
                return true;
            }
        }

        return false;
    }

    bool IsInCol(uint16_t grid[GRID_SIZE][GRID_SIZE], uint16_t col, uint16_t num)
    {
        for (std::size_t row = 0; row < GRID_SIZE; row++)
        {
            if (grid[row][col] == num)
            {
                return true;
            }
        }

        return false;
    }

    bool IsInBox(uint16_t grid[GRID_SIZE][GRID_SIZE],
                 uint16_t row,
                 uint16_t col,
                 uint16_t num)
    {
        uint16_t corner_row = row - row % SUBGRID_SIZE;
        uint16_t corner_col = col - col % SUBGRID_SIZE;

        for (std::size_t x = 0; x < SUBGRID_SIZE; x++)
        {
            for (std::size_t y = 0; y < SUBGRID_SIZE; y++)
            {
                if (grid[corner_row + x][corner_col + y] == num)
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool IsValid(uint16_t grid[GRID_SIZE][GRID_SIZE],
                 uint16_t row,
                 uint16_t col,
                 uint16_t num)
    {
        return not IsInRow(grid, row, num) and not IsInCol(grid, col, num) and
               not IsInBox(grid, row, col, num);
    }

    bool
    FindEmptyCell(uint16_t grid[GRID_SIZE][GRID_SIZE], uint16_t& row, uint16_t& col)
    {
        for (row = 0; row < GRID_SIZE; row++)
        {
            for (col = 0; col < GRID_SIZE; col++)
            {
                if (grid[row][col] == 0)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void ApplyChanges(uint16_t grid[GRID_SIZE][GRID_SIZE], Vector<State>& state)
    {
        Pair<uint16_t, uint16_t> position;
        uint16_t                 change;

        for (std::size_t i = 0; i < state.Size(); i++)
        {
            position = state[i].GetFirst();
            change   = state[i].GetSecond();

            grid[position.GetFirst()][position.GetSecond()] = change;
        }
    }

    void CopyGrid(uint16_t source[GRID_SIZE][GRID_SIZE],
                  uint16_t destination[GRID_SIZE][GRID_SIZE])
    {
        for (std::size_t i = 0; i < GRID_SIZE; i++)
        {
            for (std::size_t j = 0; j < GRID_SIZE; j++)
            {
                destination[i][j] = source[i][j];
            }
        }
    }

    void PrintGrid(uint16_t grid[GRID_SIZE][GRID_SIZE])
    {
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
                std::cout << (int)grid[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void PrintGridPythonStyle(uint16_t grid[GRID_SIZE][GRID_SIZE])
    {
        // Print the grid
        std::cout << "[[";
        for (std::size_t i = 0; i < GRID_SIZE; i++)
        {
            if (i != 0)
            {
                std::cout << " [";
            }

            for (std::size_t j = 0; j < GRID_SIZE; j++)
            {
                if (j != 0)
                {
                    std::cout << ". ";
                }
                std::cout << (int)grid[i][j];
            }

            if (i != GRID_SIZE - 1)
                std::cout << ".]" << std::endl;
        }
        std::cout << ".]]" << std::endl;
    }

    bool IsSolved(uint16_t grid[GRID_SIZE][GRID_SIZE])
    {
        for (std::size_t i = 0; i < GRID_SIZE; i++)
        {
            for (std::size_t j = 0; j < GRID_SIZE; j++)
            {
                if (grid[i][j] == 0)
                {
                    return false;
                }
            }
        }

        return true;
    }

    void PrintSubGrid(uint16_t grid[GRID_SIZE][GRID_SIZE], uint16_t row, uint16_t col)
    {
        uint16_t corner_row = row - row % SUBGRID_SIZE;
        uint16_t corner_col = col - col % SUBGRID_SIZE;

        for (std::size_t x = 0; x < SUBGRID_SIZE; x++)
        {
            for (std::size_t y = 0; y < SUBGRID_SIZE; y++)
            {
                std::cout << (int)grid[corner_row + x][corner_col + y] << " ";
            }
            std::cout << std::endl;
        }
    }
} // namespace grid
