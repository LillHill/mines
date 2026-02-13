#ifndef TYPES_H
#define TYPES_H

typedef struct pos {
  unsigned int row, col;
} pos;

typedef enum { SAFE, MINE, UNKNOWN } PLACE_STATE;
typedef enum { OK, ERROR, GAME_OVER } GAME_ACTOIN_MESG;

typedef struct {
  pos cursor;
  pos size;
  int **map;
  PLACE_STATE **map_state;
} game_state;

typedef struct pos_buf_block {
  pos pos;
  struct pos_buf_block *ptr;
} pos_buf_block;

typedef struct {
  pos_buf_block *start, *end;
  unsigned int len;
} pos_buffer;

pos_buffer init_pos_buf();
void append_pos_buf(pos_buffer *buff, pos pos);
pos pop_start_pos_buffer(pos_buffer *buff);
void print_pos_buf(pos_buffer *b);

#endif // !TYPES_H
