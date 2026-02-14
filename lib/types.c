
#include <stdio.h>
#include <stdlib.h>
typedef struct {
  unsigned int row, col;
} pos;

typedef enum { SAFE, MINE, UNKNOWN } PLACE_STATE;
typedef enum { OK, ERROR, GAME_OVER } GAME_ACTOIN_MESG;

typedef struct {
  pos cursor;
  pos size;
  int **map;
  PLACE_STATE **map_state;
  unsigned int num_of_places, num_of_places_to_search, mines_constant,
      mines_spotted;
} game_state;

typedef struct pos_buf_block {
  pos pos;
  struct pos_buf_block *ptr;
} pos_buf_block;

typedef struct {
  pos_buf_block *start, *end;
  unsigned int len;
} pos_buffer;

pos_buffer init_pos_buf() {
  pos_buffer buf;
  buf.len = 0;
  return buf;
};

void append_pos_buf(pos_buffer *buff, pos pos) {
  pos_buf_block *block = malloc(sizeof(pos_buf_block));
  block->pos = pos;

  if (buff->len == 0) {
    buff->start = block;
    buff->end = block;
    buff->len++;
    return;
  }

  buff->end->ptr = block;
  buff->end = block;
  buff->len++;
};

pos pop_start_pos_buffer(pos_buffer *buff) {

  if (buff->len == 0) {
    return (pos){0, 0};
  } else if (buff->len == 1) {
    pos pos = buff->start->pos;
    free(buff->start);
    buff->len = 0;
    return pos;
  }

  pos_buf_block *block = buff->start;
  buff->start = block->ptr;
  pos pos = block->pos;
  free(block);
  buff->len--;

  return pos;
};

void print_pos_buf(pos_buffer *b) {
  unsigned int len = b->len;
  pos_buf_block *block = b->start;
  while (len > 0) {
    printf("->(%d|%d)", block->pos.row, block->pos.col);
    block = block->ptr;
    len--;
  }
}
