#include "types.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

bool valid_pos(pos point, unsigned int row_n, unsigned int col_n)
{
    if (point.row >= row_n) {
        return false;
    }
    if (point.col >= col_n) {
        return false;
    }
    return true;
}

pos get_vec(int ori, pos point)
{
    switch (ori) {
    case 0:
        point.row -= 1;
        break;
    case 1:
        point.row -= 1;
        point.col += 1;
        break;
    case 2:
        point.col += 1;
        break;
    case 3:
        point.row += 1;
        point.col += 1;
        break;
    case 4:
        point.row += 1;
        break;
    case 5:
        point.row += 1;
        point.col -= 1;
        break;
    case 6:
        point.col -= 1;
        break;
    case 7:
        point.row -= 1;
        point.col -= 1;
        break;
    }
    return point;
}

int **map_gen(unsigned int rows_n, unsigned int cols_n, unsigned int mines_n)
{

    unsigned int place_n = rows_n * cols_n;
    if (place_n < mines_n) {
        return NULL;
    }

    int **arr = malloc(sizeof(int *) * rows_n);
    for (unsigned int i = 0; i < rows_n; i++) {
        arr[i] = malloc(sizeof(int) * cols_n);
    }

    for (unsigned int row = 0; row < rows_n; row++) {
        for (unsigned int col = 0; col < cols_n; col++) {
            arr[row][col] = 0;
        }
    }

    unsigned int mines_c_n = 0;
    while (mines_n > mines_c_n) {
        unsigned int pos = rand() % place_n;
        unsigned int row = pos % rows_n;
        unsigned int col = pos / rows_n;

        if (arr[row][col] >= 0) {
            arr[row][col] = -1;
            mines_c_n++;

            for (int i = 0; i < 8; i++) {
                struct pos point = {.row = row, .col = col};
                struct pos off_point = get_vec(i, point);
                if (!valid_pos(off_point, rows_n, cols_n)) {
                    continue;
                }
                if (arr[off_point.row][off_point.col] == -1) {
                    continue;
                }
                arr[off_point.row][off_point.col]++;
            }
        }
    }
    return arr;
}
