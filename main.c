
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include "game.h"



int main() {

    Game life;
    char menu[2]; /* variable to use for the switch menu; */
    char go_on; /* variable to use in switch case 2; */
    FILE *fp_out; /* poiter to the output file */
    FILE *fp_in; /* pointer to the input file */
    int exitgame = 0; /*flag to use for exiting the game*/
    int col = 0, row = 0; /*initializing integers that specifies the number of columns and rows of the board*/


    /*opening the file of the input*/
    fp_in = fopen("iniState.txt", "r");

    if (fp_in == NULL){
        printf("The necessary input arquive \"iniState.txt\" is not available.");
        exit(1);
    }

    /*opening the file for the output*/
    fp_out = fopen("fasedavida.txt", "w");

    /*geting the size of the board from the input file*/
    get_board_size(fp_in, &col, &row);

    config_game(&life.current, row, col); /*configuration for the board of the game.*/
    config_game(&life.next, row, col);

    seed_of_game(fp_in, &life.current); /*seed of the system*/

    printf("1 - Start continuous.\n2 - Start step by step.\n3 - Exit.\n\n\n"); /*printing the menu;*/

    scanf("%c%c",&menu[0],&menu[1]);


    switch (menu[0]) {
        case '1': //if 1, play the game continuously; press ESC to stop.

            print_game(&life.current);
            print_game_file(&life.current, fp_out, 0);
            printf("\n");
            int iteration_num = 0;

            while (exitgame != 1) {
                for (int row = 0; row < life.current.rowtotal; row++) {
                    for (int col = 0; col < life.current.coltotal; col++) {

                        rule_1(&life, row, col);
                        rule_2(&life, row, col);
                        rule_3(&life, row, col);
                        rule_4(&life, row, col);

                    }

                }
                iteration_num += 1;
                /*Switching boards, replacing the
                *current with the new one out from the iteration*/
                Population temp;
                temp = life.current;
                life.current = life.next;
                life.next = temp;
                print_game(&life.current); /*printing to the screen*/
                print_game_file(&life.current, fp_out, iteration_num); /*printing to file*/

                printf("\n");
                Sleep(1000); // pauses between the iterations so the user can see the board.
                if  (_kbhit()){
                    char ch = _getch();
                    if (ch == 27){
                        exitgame = 1;
                    }
                }
            }

            break;

        /*if 2, show each interaction everytime the user hits "enter".
         *press ESC to exit*/

        case '2':
            print_game(&life.current);
            print_game_file(&life.current, fp_out, 0);
            printf("\n");
            int iteration_num2 = 0;

            do {

                for (int row = 0; row < life.current.rowtotal; row++) {
                    for (int col = 0; col < life.current.coltotal; col++) {

                        rule_1(&life, row, col);
                        rule_2(&life, row, col);
                        rule_3(&life, row, col);
                        rule_4(&life, row, col);

                    }

                }
                iteration_num2 += 1;
                /*Switching boards*/
                Population temp;
                temp = life.current;
                life.current = life.next;
                life.next = temp;
                print_game(&life.current);
                print_game_file(&life.current, fp_out, iteration_num2);

                printf("\n");
                go_on = _getch();


            } while (go_on != 27);
            break;

        case '3':
            exit(1);
            break;

        default:
            break;
    }
    /*closing files*/
    fclose(fp_in);
    fclose(fp_out);

    return 0;
}
