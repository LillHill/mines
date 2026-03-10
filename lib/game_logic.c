#include "mapgen.h"
#include "types.h"
#include <stdbool.h>
#include <stdlib.h>

game_state *init_game(pos size, char mine_percentage, pos old_cursor)
{
    unsigned int num_places = size.col * size.row;
    unsigned int num_mines = (num_places * mine_percentage) / 100;
    game_state *g = malloc(sizeof(game_state));
    g->size = size;
    g->mines_constant = num_mines;
    g->num_of_places = num_places;
    g->num_of_places_to_search = num_places;
    g->mines_spotted = 0;
    g->cursor = old_cursor;
    g->map = map_gen(size.row, size.col, num_mines);

    PLACE_STATE **ptr_rows = malloc(sizeof(PLACE_STATE *) * size.row);
    for (unsigned int row = 0; row < size.row; row++) {
        ptr_rows[row] = malloc(sizeof(PLACE_STATE) * size.col);
        for (unsigned int col = 0; col < size.col; col++) {
            ptr_rows[row][col] = UNKNOWN;
        }
    }

    g->map_state = ptr_rows;
    return g;
}

void free_game(game_state *g)
{
    for (unsigned int row = 0; row < g->size.row; row++) {
        free(g->map_state[row]);
        free(g->map[row]);
    }
    free(g->map);
    free(g->map_state);
    free(g);
}

bool pos_in_limits(game_state *g, pos p)
{
    pos limits = g->size;
    if (p.row >= limits.row) {
        return false;
    }
    if (p.col >= limits.col) {
        return false;
    }
    return true;
}

GAME_ACTOIN_MESG step_on_place(game_state *g)
{
    if (!pos_in_limits(g, g->cursor)) {
        return ERROR;
    }

    pos place = g->cursor;
    PLACE_STATE state = g->map_state[place.row][place.col];
    if (state == MINE || state == SAFE) {
        return OK;
    }

    int contents_of_pos = g->map[place.row][place.col];
    if (contents_of_pos == -1) {
        return GAME_OVER;
    }

    // g->map_state[place.row][place.col] = SAFE;

    pos_buffer buffer = init_pos_buf();
    append_pos_buf(&buffer, place);

    do {
        // printf("buff len %d\n", buffer.len);
        // print_pos_buf(&buffer);
        // puts("\n");
        pos eval = pop_start_pos_buffer(&buffer);
        if (g->map_state[eval.row][eval.col] == SAFE) {
            continue;
        }

        // MINES are surrounded by != 0 places and these places do not spam new
        // instances...
        g->map_state[eval.row][eval.col] = SAFE;
        g->num_of_places_to_search--;
        if (g->map[eval.row][eval.col] == 0) {
            for (unsigned char ori = 0; ori < 8; ori++) {
                pos off_pos = get_vec(ori, eval);
                // printf("eval%d %d\n", off_pos.row, off_pos.col);
                if (!pos_in_limits(g, off_pos)) {
                    // puts("out");
                    continue;
                } else if (g->map_state[off_pos.row][off_pos.col] == SAFE) {
                    // puts("igSafe");
                    continue;
                }
                append_pos_buf(&buffer, off_pos);
            }
        }
    } while (buffer.len > 0);
    return OK;
}
