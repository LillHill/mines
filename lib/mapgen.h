#ifndef MAPGEN_H
#define MAPGEN_H

#include <stdbool.h>
#include "types.h"

bool valid_pos(pos point, unsigned int row_n, unsigned int col_n);
pos get_vec(int ori, pos point);
int **map_gen(unsigned int rows_n, unsigned int cols_n, unsigned int mines_n);

#endif // MAPGEN_H
