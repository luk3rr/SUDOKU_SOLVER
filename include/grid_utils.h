/*
 * Filename: grid_utils.h
 * Created on: April 13, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef GRID_UTILS_H_
#define GRID_UTILS_H_

#include <cstdint>

#include "constants.h"
#include "vector.h"
#include <iostream>

/**
 * @brief Namespace containing utility functions for the grid
 **/
namespace grid
{
    /**
     * @brief Check if the grid is valid
     * @param grid Grid to check
     * @return True if the grid is valid, false otherwise
     **/
    bool GridIsValid(uint16_t grid[GRID_SIZE][GRID_SIZE]);

    /**
     * @brief Find an empty position in the grid
     * @param grid Grid to find the empty position
     * @param row Row of the empty position
     * @param col Column of the empty position
     * @return True if an empty position was found, false otherwise
     **/
    bool
    FindEmptyCell(uint16_t grid[GRID_SIZE][GRID_SIZE], uint16_t& row, uint16_t& col);

    /**
     * @brief Apply the changes to the grid
     * @param grid Grid to apply the changes
     * @param changes Changes to apply
     */
    void ApplyChanges(uint16_t grid[GRID_SIZE][GRID_SIZE], Vector<State>& changes);

    /**
     * @brief Check if a number is in a row
     * @param grid Grid to check
     * @param row Row to check
     * @param num Number to check
     * @return True if the number is in the row, false otherwise
     **/
    bool IsInRow(uint16_t grid[GRID_SIZE][GRID_SIZE], uint16_t row, uint16_t num);

    /**
     * @brief Check if a number is in a column
     * @param grid Grid to check
     * @param col Column to check
     * @param num Number to check
     * @return True if the number is in the column, false otherwise
     **/
    bool IsInCol(uint16_t grid[GRID_SIZE][GRID_SIZE], uint16_t col, uint16_t num);

    /**
     * @brief Check if a number is in a box
     * @param grid Grid to check
     * @param boxStartRow Row of the box to check
     * @param boxStartCol Column of the box to check
     * @param num Number to check
     * @return True if the number is in the box, false otherwise
     **/
    bool IsInBox(uint16_t grid[GRID_SIZE][GRID_SIZE],
                 uint16_t boxStartRow,
                 uint16_t boxStartCol,
                 uint16_t num);

    /**
     * @brief Check if a number is valid in a position
     * @param grid Grid to check
     * @param row Row to check
     * @param col Column to check
     * @param num Number to check
     * @return True if the number is valid in the position, false otherwise
     **/
    bool IsValid(uint16_t grid[GRID_SIZE][GRID_SIZE],
                 uint16_t row,
                 uint16_t col,
                 uint16_t num);

    /**
     * @brief Copy the grid to a new grid
     * @param source Grid to copy
     * @param destination New grid
     */
    void CopyGrid(uint16_t source[GRID_SIZE][GRID_SIZE],
                  uint16_t destination[GRID_SIZE][GRID_SIZE]);

    /**
     * @brief Print the grid
     * @param grid Grid to print
     **/
    void PrintGrid(uint16_t grid[GRID_SIZE][GRID_SIZE]);
    void PrintGridPythonStyle(uint16_t grid[GRID_SIZE][GRID_SIZE]);
    void PrintGridLineStyle(uint16_t grid[GRID_SIZE][GRID_SIZE]);

    /**
     * @brief Print the subgrid
     * @param grid Grid to print
     * @param row Row of the subgrid
     * @param col Column of the subgrid
     **/
    void PrintSubGrid(uint16_t grid[GRID_SIZE][GRID_SIZE], uint16_t row, uint16_t col);

    /**
     * @brief Check if the current grid is solved
     * @param grid Grid to check
     * @return True if the grid is solved, false otherwise
     **/
    bool IsSolved(uint16_t grid[GRID_SIZE][GRID_SIZE]);

} // namespace grid

#endif // GRID_UTILS_H_
