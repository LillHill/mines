#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include "types.h"
#include <stdbool.h>
bool pos_in_limits(game_state *g, pos p);
game_state *init_game(pos size, char mine_percentage, pos old_cursor);
void free_game(game_state *g);
GAME_ACTOIN_MESG step_on_place(game_state *g);
#endif // !GAME_LOGIC_H
