#ifndef CODE_BOARD
#define CODE_BOARD

#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <time.h>
#include "./globals.c"

#define YABL_BOARD_WIDTH 20
#define YABL_BOARD_HEIGHT 4

//#
struct winsize size;
const int m = YABL_BOARD_WIDTH, n = YABL_BOARD_HEIGHT;
char buffer[YABL_BOARD_WIDTH][YABL_BOARD_HEIGHT] = {};

void printBoard();
void redraw_screen();
void updateBoard();
int endGame();
/*
int main()
{
    int i, j;
    for(i = 0; i < m; i++)
    {
        for(j = 0; j < n; j++)
        {
            buffer[i][j] = ' ';
        }
    }
    printBoard();
    while(endGame() != 1)
    {
        signal( SIGWINCH, printBoard);
        //updateBoard();
    }
    return 0;
}
*/

void printBoard()
{
    int i, j;

    fprintf(stdout, "\x1b[H\x1b[2J\x1b[3J");

    ioctl(0, TIOCGWINSZ, &size);

    if(size.ws_col < m * 5 || size.ws_row < n * 6)
    {
        redraw_screen();
    }

    else
    {
        //prints the board according to dimensions of the preamble, when the window size can accommodate it
        String* s = gettoken(i,j);

        for(i = 0; i < size.ws_col; i++)
        {
            fprintf(stdout, "\x1b[32m*");
            fflush(stdout);
        }
        
        fprintf(stdout, "\x1b[3B\x1b[1000D");
        fflush(stdout);

        
        for(j = 0; j < n; j++)
        { 
            for(i = 0; i < (size.ws_col - m * 5) / 2; i++)
            {
                fprintf(stdout, "\x1b[1C");
                fflush(stdout);
            }

            if(YABL_BOARD_HEIGHT == 1 && YABL_BOARD_WIDTH == 1){
                fprintf(stdout, "\x1b[31m\x1b[2A┌───┐\x1b[1B\x1b[5D│ %s │\x1b[1B\x1b[5D└───┘", s->string);

                fflush(stdout);
            }
            else if(YABL_BOARD_HEIGHT == 1 && YABL_BOARD_WIDTH != 1){
                for(i = 0; i < m; i++)
                {
                    if(i == 0){
                        fprintf(stdout, "\x1b[31m\x1b[2A┌───┬\x1b[1B\x1b[5D│ %s │\x1b[1B\x1b[5D└───┴", s->string);
                    }
                    else if (i == m - 1){
                        fprintf(stdout, "\x1b[31m\x1b[2A───┐\x1b[1B\x1b[4D %s │\x1b[1B\x1b[4D───┘", s->string);
                    }
                    else{
                        fprintf(stdout, "\x1b[31m\x1b[2A───┬\x1b[1B\x1b[4D %s │\x1b[1B\x1b[4D───┴", s->string);
                    }
                    
                }
                fflush(stdout);
            }
            else{
                if(j == 0){
                    for(i = 0; i < m; i++){
                        if(i == 0){
                            if(YABL_BOARD_WIDTH == 1){
                                fprintf(stdout, "\x1b[31m\x1b[2A┌───┐\x1b[1B\x1b[5D│ %s │\x1b[1B\x1b[5D├───┤", s->string);
                            }
                            else{
                                fprintf(stdout, "\x1b[31m\x1b[2A┌───┬\x1b[1B\x1b[5D│ %s │\x1b[1B\x1b[5D├───┼", s->string);
                            }
                            
                        }
                        else if (i == m - 1){
                            fprintf(stdout, "\x1b[31m\x1b[2A───┐\x1b[1B\x1b[4D %s │\x1b[1B\x1b[4D───┤", s->string);
                        }
                        else{
                            fprintf(stdout, "\x1b[31m\x1b[2A───┬\x1b[1B\x1b[4D %s │\x1b[1B\x1b[4D───┼", s->string);
                        }
                    }
                }else if(j == n - 1){
                    for(i = 0; i < m; i++){
                        if(i == 0){
                            if(YABL_BOARD_WIDTH == 1){
                                fprintf(stdout, "\x1b[31m\x1b[3A├───┤\x1b[1B\x1b[5D│ %s │\x1b[1B\x1b[5D└───┘", s->string);
                            }else{
                                fprintf(stdout, "\x1b[31m\x1b[3A├───┼\x1b[1B\x1b[5D│ %s │\x1b[1B\x1b[5D└───┴", s->string);
                            }

                            
                        }
                        else if (i == m - 1){
                            fprintf(stdout, "\x1b[31m\x1b[2A───┤\x1b[1B\x1b[4D %s │\x1b[1B\x1b[4D───┘", s->string);
                        }
                        else{
                            fprintf(stdout, "\x1b[31m\x1b[2A───┼\x1b[1B\x1b[4D %s │\x1b[1B\x1b[4D───┴", s->string);
                        }
                    }
                }
                else{
                    for(i = 0; i < m; i++){
                        if(i == 0){
                            if(YABL_BOARD_WIDTH == 1){
                                fprintf(stdout, "\x1b[31m\x1b[3A├───┤\x1b[1B\x1b[5D│ %s │\x1b[1B\x1b[5D├───┤", s->string);
                            }else{
                                fprintf(stdout, "\x1b[31m\x1b[3A├───┼\x1b[1B\x1b[5D│ %s │\x1b[1B\x1b[5D├───┼", s->string);
                            }

                            
                        }
                        else if (i == m - 1){
                            fprintf(stdout, "\x1b[31m\x1b[2A───┤\x1b[1B\x1b[4D %s │\x1b[1B\x1b[4D───┼", s->string);
                        }
                        else{
                            fprintf(stdout, "\x1b[31m\x1b[2A───┼\x1b[1B\x1b[4D %s │\x1b[1B\x1b[4D───┼", s->string);
                        }
                    }
                }
            }

            fprintf(stdout, "\x1b[3B\x1b[1000D");
            fflush(stdout);
        }

        destroyString(s);

        for(i = 0; i < size.ws_col; i++)
        {
            fprintf(stdout, "\x1b[32m*");
            fflush(stdout);
        }
    }
}

void updateBoard()
{
    int column, row;
    int i, j;
    char c;

    fprintf(stdout, "Update token board positions by providing an integer for column(x), one for row(y), aswell as a token to be printed:\x1b[0m ");
    scanf(" %d %d %c", &column, &row, &c);
    column = column -1;
    row = row - 1;
    buffer[column][row] = c;
    fprintf(stdout, "\x1b[32m");
    fprintf(stdout, "Column %d, row %d has been updated with your new token board position.", column, row);
    fprintf(stdout, "\x1b[0;0H\x1b[32m");
    for(i = 0; i < n; i++)
    {
        fprintf(stdout, "\x1b[3B");
    }
    fprintf(stdout, "\x1b[1B\x1b[0J");
    printBoard();
}

int endGame()
{
    return 0;
}

void redraw_screen()
{
    ioctl(0, TIOCGWINSZ, &size);
    fprintf(stdout, "\x1b[H\x1b[2J\x1b[3J");
    int i;
    int width = size.ws_col;
    int height = size.ws_row;

    for(i = 0; i < height / 2; i++)
    {
        fprintf(stdout, "\x1b[1B");
        fflush(stdout);
    }
    fprintf(stdout, "\x1b[31m🡄 You need to increase the width of the terminal to accommodate the chosen board size.");
    fflush(stdout);

    fprintf(stdout, "\x1b[H");
    fflush(stdout);

    for(i = 0; i < (width - 88) / 2; i++)
    {
        fprintf(stdout, "\x1b[1C");
        fflush(stdout);
    }
    fprintf(stdout, "\x1b[31m🡅 You need to increase the height of the terminal to accommodate the chosen board size.🡅");
    fflush(stdout);
}

//#

#endif /* ifndef CODE_BOARD */
