

#ifndef game_h
#define game_h

#include <stdio.h>
#include <stdlib.h>

/*struct that contains a status of a cell (dead - 0 or alive - 1)*/
typedef struct {
    int status;
}Cell;

/*Struct that contains the board pieces cells, number of columns and rows.*/
typedef struct {
    Cell **cells;
    int rowtotal;
    int coltotal;
} Population;

/*struct that contains the boards of the game, current and the next generation*/
typedef struct{
    Population current;
    Population next;
} Game;

/*Function to allocate memory for a game of size rowtotal*coltotal.*/
void config_game (Population *board, int rowtotal, int coltotal);

void get_board_size (FILE *fp, int *x, int *y);

void rule_1 (Game *game, int row, int col);

void rule_2 (Game *game, int row, int col);

void rule_3 (Game *game, int row, int col);

void rule_4 (Game *game, int row, int col);

void print_game (Population *board);

void print_game_file (Population *board, FILE *fp, int iteration);

void seed_of_game (FILE *fp, Population *board);

void seed_test (Population *board);

void seed_test_random (Population *board);

#endif /* game_h */
