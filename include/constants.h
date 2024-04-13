/*
 * Filename: constants.h
 * Created on: April 12, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <cstdint>

constexpr uint8_t GRID_SIZE    = 9;
constexpr uint8_t SUBGRID_SIZE = 3;

enum class Algorithm : char
{
    BFS    = 'B',
    IDDFS  = 'I',
    UCS    = 'U',
    A_STAR = 'A',
    GBFS   = 'G',
};

#endif // CONSTANTS_H_
