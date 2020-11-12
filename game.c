
#include "game.h"
#include <time.h>
#include <stdlib.h>

/*function to count the number of rows and columns from the input file.*/
void get_board_size (FILE *fp, int *x, int *y){
    int col = 0;
    int row = 1; /*the first loop already  counts 1 row*/
    char c = fgetc(fp); //function that gets the next character from the specified file and advances the position           indicator for the file.

    while (c != '\n') { /*reading the number of columns*/
        if (c != ' ') {
            col += 1;
        }
        c = fgetc(fp);
    }
    *x = col;

    c = fgetc(fp);

    while (c != EOF) { /*reading the number of rows*/
        if (c == '\n') {
            row += 1;
        }
        c = fgetc(fp);
    }
    *y = row;
}

/*memory allocation for the game*/
void config_game (Population *board, int rowtotal, int coltotal){
   board->cells = (Cell**)malloc(rowtotal*sizeof(Cell *));
    if (board->cells == NULL) {
        exit(-1);
    }
    for (int i = 0; i < rowtotal; i++) {
        board->cells[i] = (Cell*)malloc(coltotal*sizeof(Cell));
    }
    board->rowtotal = rowtotal;
    board->coltotal = coltotal;

}

/*function that gets the initial state from the user throught the file input
 * it receives a pointer to the file, and a board.
 */
void seed_of_game (FILE *fp, Population *board) {
    int col = 0;
    int row = 0;
    rewind(fp);
    char c = fgetc(fp); //function that gets the next character from the specified file and advances the position           indicator for the file.

    while (c != EOF) { /*reading the file while is not the end of the file*/
        if (c == '0' || c == '1') {
            board->cells[row][col].status = c-'0'; /*filling up the board with the values from the input*/
            col += 1;
        }
        else if (c == '\n') { /*identifying a new row*/
            col = 0;
            row += 1;
        }

        c = fgetc(fp);
    }

}

/*Function that counts how many live neighbors a cell has*/
int count_alive (Population *board, int row, int col){
    int countalive = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col +1; j++) {
            int ivalid = i>=0 && i< board->rowtotal; /*boolean to check border cells*/
            int jvalid = j>=0 && j< board->coltotal;
            int samecell = i == row && j == col; /*Boolean to check if is the current cell*/
            if (ivalid && jvalid && !samecell) { /*If everything is valid and this is not the same cell*/
                if (board->cells[i][j].status == 1) { /*If neighbor cell is alive, count +1*/
                    countalive +=1;
                }

            }
        }
    }
    return countalive;
}
/*Any live cell with fewer than two live neighbors dies, as if by underpopulation.*/
void rule_1 (Game *game, int row, int col){
    int current_cell = game->current.cells[row][col].status; /*check status of current cell*/
    int aliveadj = count_alive(&game->current, row, col); /*counts how many neighbors cells are alive*/

    if (current_cell && aliveadj < 2) {
        game->next.cells[row][col].status = 0;
    }
}
/* Any live cell with two or three live neighbors lives on to the next generation.*/
void rule_2 (Game *game, int row, int col){
    int current_cell = game->current.cells[row][col].status; /*check status of current cell*/
    int aliveadj = count_alive(&game->current, row, col);
    if (current_cell && (aliveadj == 2 || aliveadj == 3)) {
        game->next.cells[row][col].status = 1;
    }
}
/*Any live cell with more than three live neighbors dies, as if by overpopulation.*/
void rule_3 (Game *game, int row, int col){
    int current_cell = game->current.cells[row][col].status; /*check status of current cell*/
    int aliveadj = count_alive(&game->current, row, col);
    if (current_cell && aliveadj > 3) {
        game->next.cells[row][col].status = 0;
    }
}
/*Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.*/
void rule_4 (Game *game, int row, int col){
     int current_cell = game->current.cells[row][col].status; /*check status of current cell*/
    int aliveadj = count_alive(&game->current, row, col);
    if (!current_cell && aliveadj == 3) {
        game->next.cells[row][col].status = 1;
    }
    else if (!current_cell && (aliveadj != 3)) {
            game->next.cells[row][col].status = 0;
        }
}

void print_game (Population *board){
    for (int row = 0; row < board->rowtotal; row++) {
        for (int col = 0; col < board->coltotal ; col++){
            printf("%d ", board->cells[row][col].status);
        }
        printf("\n");
    }
}

/*function to print to the file output the iterations of the game and they respective board*/
void print_game_file (Population *board, FILE *fp, int iteration){
    fprintf(fp, "Iteration %d: \n\n", iteration);
    for (int row = 0; row < board->rowtotal; row++) {
        for (int col = 0; col < board->coltotal ; col++){
            fprintf(fp, "%d ", board->cells[row][col].status);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");
}

/*filling up the board initially randomically for testing
 *If row even and col odd, set the value for 1
 *otherwise, set it to 0 */
void seed_test (Population *board){

    for (int row = 0; row < board->rowtotal; row++) {
        for (int col = 0; col < board->coltotal ; col++) {
            if (row % 2 == 0)  {
                if (col % 2 != 0) {
                     board->cells[row][col].status = 1;
                }
                else board->cells[row][col].status = 0;
            }

            if (row % 2 != 0) {
                if (col % 2 == 0) {
                    board->cells[row][col].status = 1;
                }
                else  board->cells[row][col].status = 0;
            }

        }
    }

}
/*Setting values 1 and 0 with random function*/
void seed_test_random (Population *board){
    srand((unsigned int)time(NULL));   // Initialization.

    for (int row = 0; row < board->rowtotal; row++) {
        for (int col = 0; col < board->coltotal ; col++) {
            int r = rand();  // Returns a pseudo-random integer between 0 and RAND_MAX.
            if (r % 2 == 0)  {
                    board->cells[row][col].status = 1;
                }
                else board->cells[row][col].status = 0;
            }
        }
}
