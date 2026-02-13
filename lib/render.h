#ifndef RENDER_H
#define RENDER_H
#include "types.h"
void clear();
char *render_line(unsigned int row, game_state *g);
void print_map(game_state *g);

#endif // !RENDER_H
