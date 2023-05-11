#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <time.h>

struct winsize size;
char buffer[20][4] = {};

void printBoard(int, int);
void redraw_screen_width();
void redraw_screen_height();

void updateBoard(int);

int endGame();

int main()
{
    //m,n = !iXj
    printBoard(20, 4);
    while(endGame() != 1)
    {
        updateBoard(4);
    }
    return 0;
}

void printBoard(int m, int n)
{
    ioctl(0, TIOCGWINSZ, &size);
    int width = size.ws_col;
    int height = size.ws_row;
    int i, j;

    while(size.ws_col < m * 5)
    {
        signal( SIGWINCH, redraw_screen_width);
    }

    while(size.ws_row < n * 6)
    {
        signal(SIGWINCH, redraw_screen_height);
    }

    fprintf(stdout, "\x1b[H\x1b[2J\x1b[3J");

    //prints the board according to dimensions of the preamble, when the window size can accommodate it
    for(j = 0; j < n; j++)
    { 
        for(i = 0; i < (width - m * 5) / 2; i++)
        {
            fprintf(stdout, "\x1b[1C");
        }

        for(i = 0; i < m; i++)
        {
            fprintf(stdout, "\x1b[0m\x1b[2A╔═══╗\x1b[1B\x1b[5D║   ║\x1b[1B\x1b[5D╚═══╝");
        }

        fprintf(stdout, "\x1b[3B\x1b[1000D");
    }

    //saves cursor position in the first tile position and then resets cursor position for next input
    fprintf(stdout, "\x1b[0;0H");
    for(i = 0; i < (width - m * 5) / 2; i++)
    {
        fprintf(stdout, "\x1b[1C");
    }
    fprintf(stdout, "\x1b[2C\x1b[1B\x1b[s\x1b[1000D\x1b[1000A");
    for(i = 0; i < n; i++)
    {
        fprintf(stdout, "\x1b[3B");
    }

    for(i = 0; i < size.ws_col; i++)
    {
        fprintf(stdout, "\x1b[32m*");
    }

    /*int i, j, m, n;
    m = 3;
    for(i = 0; i < m; i++)
    {
        sleep(1);
        printf("╔═══╗\n║   ║\n╚═══╝\x1b[1A");
        /*delay(1000);*//*
    }
    printf("\n**************************");*/

    /*
    fprintf( stdout, "Loading...\n" );
    sleep(1);
    fprintf( stdout, "1 procent" );
    fflush(stdout);
    sleep(1);
    fprintf( stdout, "\x1b[900D2 procent" );
    */

    /*
    fprintf( stdout, "Loading...\n" );
    int i;
    for(i=0; i<10; i++)
    {
        sleep(1);
        fprintf( stdout, "\x1b[900D%d procent", i );
        fflush(stdout);
    }
    */
}

void updateBoard(int dim_y)
{
    int column, row;
    int i, j;

    fprintf(stdout, "Update token board positions by providing an integer for column(x) and one for row(y) aswell:\x1b[0m ");
    scanf(" %d %d", &column, &row);
    fprintf(stdout, "\x1b[32m");
    fprintf(stdout, "Column %d, row %d has been updated with your new token board position.", column, row);
    fprintf(stdout, "\x1b[u");
    for(i = 1; i < column; i++)
    {
        fprintf(stdout, "\x1b[5C");
    }
    for(j = 1; j < row; j++)
    {
        fprintf(stdout, "\x1b[3B");
    }
    fprintf(stdout, "X");
    fprintf(stdout, "\x1b[0;0H\x1b[32m");
    for(i = 0; i < dim_y; i++)
    {
        fprintf(stdout, "\x1b[3B");
    }
    fprintf(stdout, "\x1b[1B\x1b[0J");
}

int endGame()
{
    return 0;
}

void redraw_screen_width()
{
    ioctl(0, TIOCGWINSZ, &size);
    fprintf(stdout, "\x1b[H\x1b[2J\x1b[3J");
    int i;
    int height = size.ws_row;
    for(i = 0; i < height / 2; i++)
    {
        fprintf(stdout, "\x1b[1B");
    }
    fprintf(stdout, "\x1b[31m🡄 You need to increase the width of the terminal to accommodate the chosen board size.");
    fflush(stdout);
}

void redraw_screen_height()
{
    ioctl(0, TIOCGWINSZ, &size);
    fprintf(stdout, "\x1b[H\x1b[2J\x1b[3J");
    int i;
    int width = size.ws_col;
    for(i = 0; i < (width - 88) / 2; i++)
    {
        fprintf(stdout, "\x1b[1C");
    }
        fprintf(stdout, "\x1b[31m🡅 You need to increase the height of the terminal to accommodate the chosen board size.🡅");
        fflush(stdout);
}