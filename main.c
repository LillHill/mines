#include "lib/game_logic.h"
#include "lib/render.h"
#include "lib/types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("not enought arguments");
    return EXIT_FAILURE;
  }

  int rows_n = atoi(argv[1]);
  int cols_n = atoi(argv[2]);
  char mine_density = atoi(argv[3]);

  if (rows_n <= 0 || cols_n <= 0 ||
      (mine_density >= 100 || mine_density <= 0)) {
    printf("Invalid arguments");
    return EXIT_FAILURE;
  }

  srand(time(NULL));
  game_state *game =
      init_game((pos){.row = rows_n, .col = cols_n}, mine_density, (pos){0, 0});
  pos cursor;
  bool invalid = true;
  do {
    cursor.row = rand() % rows_n;
    cursor.col = rand() % cols_n;
    if (!pos_in_limits(game, cursor)) {
      continue;
    } else if (game->map[cursor.row][cursor.col] != 0) {
      continue;
    }
    invalid = false;
  } while (invalid);

  game->cursor = cursor;
  step_on_place(game);

  print_map(game);

  struct termios old_tio, new_tio;
  unsigned char c;

  /* get the terminal settings for stdin */
  tcgetattr(STDIN_FILENO, &old_tio);

  /* we want to keep the old setting to restore them a the end */
  new_tio = old_tio;

  /* disable canonical mode (buffered i/o) and local echo */
  new_tio.c_lflag &= (~ICANON & ~ECHO);

  /* set the new settings immediately */
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

  do {
    c = getchar();
    if (c == 27) { // ESC character
      unsigned char c2, c3;
      c2 = getchar();
      c3 = getchar();
      if (c2 != '[') {
        printf("special character unsupported %d %d", c2, c3);
      } else {
        switch (c3) {
        case 'A':
          printf("\"arr up");
          game->cursor.row--;
          break;
        case 'B':
          printf("\"arr down");
          game->cursor.row++;
          break;
        case 'C':
          printf("\"arr right");
          game->cursor.col++;
          break;
        case 'D':
          printf("\"arr left");
          game->cursor.col--;
          break;
        }
        print_map(game);
      }
    }

    else if (c == ' ') {
      GAME_ACTOIN_MESG ret = step_on_place(game);
      print_map(game);
      switch (ret) {

      case OK:
        puts("OK/n");
        break;
      case ERROR:
        puts("ERROR\n");
        break;
      case GAME_OVER:
        puts("GAME OVER");
        break;
      }
    } else if (c == 'b') {
      PLACE_STATE *place = &game->map_state[game->cursor.row][game->cursor.col];
      if (*place == MINE) {
        *place = UNKNOWN;
      } else if (*place == UNKNOWN) {
        *place = MINE;
      }
      print_map(game);
    }

    printf("\"%c", c);
  } while (c != 'q');

  /* restore the former settings */
  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

  return 0;
}
