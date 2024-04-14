/*
 * Filename: constants.h
 * Created on: April 12, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <cstdint>

#include "pair.h"

constexpr uint16_t GRID_SIZE    = 9;
constexpr uint16_t SUBGRID_SIZE = 3;

enum class Algorithm : char
{
    BFS    = 'B',
    IDDFS  = 'I',
    UCS    = 'U',
    A_STAR = 'A',
    GBFS   = 'G',
};

using State = Pair<Pair<uint16_t, uint16_t>, uint16_t>;

#endif // CONSTANTS_H_
