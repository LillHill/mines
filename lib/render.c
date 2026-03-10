// ===========================
// ANSI Escape Codes (Cursor, Screen, Colors)
// ===========================

// Base
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ESC "\033"  // Escape (ASCII 27)
#define CSI "\033[" // Control Sequence Introducer

// ———————————————————
// Cursor Movement
// ———————————————————
#define CURSOR_UP(n) CSI #n "A"    // Move cursor up n lines
#define CURSOR_DOWN(n) CSI #n "B"  // Move cursor down n lines
#define CURSOR_RIGHT(n) CSI #n "C" // Move cursor right n cols
#define CURSOR_LEFT(n) CSI #n "D"  // Move cursor left n cols

#define CURSOR_POS(r, c) CSI #r ";" #c "H" // Move to row r, col c
#define CURSOR_HOME CSI "H"                // Home: row 1, col 1

// Save / Restore
#define SAVE_CURSOR CSI "s"    // Save cursor pos
#define RESTORE_CURSOR CSI "u" // Restore cursor pos

// ———————————————————
// Screen & Line Control
// ———————————————————
#define CLEAR_SCREEN CSI "2J" // Clear screen
#define CLEAR_LINE CSI "K"    // Clear line from cursor to end
#define ERASE_LINE CSI "2K"   // Erase entire line

// ———————————————————
// Foreground Colors (Normal & Bright)
// ———————————————————
#define FG_BLACK CSI "30m"
#define FG_RED CSI "31m"
#define FG_GREEN CSI "32m"
#define FG_YELLOW CSI "33m"
#define FG_BLUE CSI "34m"
#define FG_MAGENTA CSI "35m"
#define FG_CYAN CSI "36m"
#define FG_WHITE CSI "37m"

#define FG_BRIGHT_BLACK CSI "90m"
#define FG_BRIGHT_RED CSI "91m"
#define FG_BRIGHT_GREEN CSI "92m"
#define FG_BRIGHT_YELLOW CSI "93m"
#define FG_BRIGHT_BLUE CSI "94m"
#define FG_BRIGHT_MAGENTA CSI "95m"
#define FG_BRIGHT_CYAN CSI "96m"
#define FG_BRIGHT_WHITE CSI "97m"

// ———————————————————
// Background Colors (Normal & Bright)
// ———————————————————
#define BG_BLACK CSI "40m"
#define BG_RED CSI "41m"
#define BG_GREEN CSI "42m"
#define BG_YELLOW CSI "43m"
#define BG_BLUE CSI "44m"
#define BG_MAGENTA CSI "45m"
#define BG_CYAN CSI "46m"
#define BG_WHITE CSI "47m"

#define BG_BRIGHT_BLACK CSI "100m"
#define BG_BRIGHT_RED CSI "101m"
#define BG_BRIGHT_GREEN CSI "102m"
#define BG_BRIGHT_YELLOW CSI "103m"
#define BG_BRIGHT_BLUE CSI "104m"
#define BG_BRIGHT_MAGENTA CSI "105m"
#define BG_BRIGHT_CYAN CSI "106m"
#define BG_BRIGHT_WHITE CSI "107m"

// ———————————————————
// Reset / Styles
// ———————————————————
#define ANSI_RESET CSI "0m" // Reset colors/styles
#define STYLE_BOLD CSI "1m"
#define STYLE_UNDERLINE CSI "4m"
#define STYLE_INVERSE CSI "7m"

void update_used_len(unsigned int *len, char *buff, unsigned int alloc_len)
{
    while (buff[*len] != '\0' && *len < alloc_len) {
        (*len)++;
    }
}

const unsigned int MAX_INCREASE = 20;
void clear() { printf(CLEAR_SCREEN CURSOR_HOME); }
char *render_line(unsigned int row, game_state *g)
{
    if (g->size.row <= row) {
        return NULL;
    }

    int *line = g->map[row];
    PLACE_STATE *line_state = g->map_state[row];

    PLACE_STATE state;
    int place;
    unsigned int used_len = 0;
    unsigned int alloc_len = sizeof(char) * g->size.col;
    char *out = malloc(alloc_len);
    out[0] = '\0';

    for (unsigned int col = 0; col < g->size.col; col++) {
        update_used_len(&used_len, out, alloc_len);
        if (used_len + MAX_INCREASE >= alloc_len) {
            alloc_len += sizeof(char) * MAX_INCREASE;
            out = realloc(out, alloc_len);
            if (out == NULL)
                exit(100);
        }

        place = line[col];
        state = line_state[col];

        if (row == g->cursor.row && col == g->cursor.col) {
            strcat(out, BG_MAGENTA FG_WHITE "#" ANSI_RESET);
            continue;
        }

        switch (state) {
        case UNKNOWN:
            strcat(out, BG_WHITE " " ANSI_RESET);
            break;
        case MINE:
            strcat(out, BG_BRIGHT_RED FG_WHITE "#" ANSI_RESET);
            break;
        case SAFE:
            switch (place) {
            case 0:
                strcat(out, FG_BLACK " ");
                break;
            case 1:
                strcat(out, FG_BLUE "1");
                break;
            case 2:
                strcat(out, FG_BRIGHT_BLUE "2");
                break;
            case 3:
                strcat(out, FG_GREEN "3");
                break;
            case 4:
                strcat(out, FG_BRIGHT_GREEN "4");
                break;
            case 5:
                strcat(out, FG_MAGENTA "5");
                break;
            case 6:
                strcat(out, FG_BRIGHT_MAGENTA "6");
                break;
            case 7:
                strcat(out, FG_YELLOW "7");
                break;
            case 8:
                strcat(out, FG_RED "8");
                break;
            }
        }
    }

    strcat(out, ANSI_RESET);

    return out;
}

void print_map(game_state *g)
{
    clear();
    char *line;
    for (unsigned int row = 0; row < g->size.row; row++) {
        line = render_line(row, g);
        puts(line);
        free(line);
    }
}
