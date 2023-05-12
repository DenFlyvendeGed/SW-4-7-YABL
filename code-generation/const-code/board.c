#ifndef CODE_BOARD
#define CODE_BOARD

#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include "./globals.c"
#include "./string.c"

#define YABL_BOARD_WIDTH 20
#define YABL_BOARD_HEIGHT 4
#define YABL_BOARD_WRITE_WIDTH 20
#define YABL_BOARD_WRITE_HEIGHT 4

//#
struct winsize size;
const int m = YABL_BOARD_WIDTH, n = YABL_BOARD_HEIGHT;
char buffer[YABL_BOARD_WIDTH][YABL_BOARD_HEIGHT] = {};

void printBoard();
void redraw_screen();
void updateBoard();
int endGame();

void NULL_FUNC(int i){}

#define EDGE_WIDTH_CORNER_FAR   (1 << 0)
#define EDGE_WIDTH_CORNER_NEAR  (1 << 1)
#define EDGE_WIDTH_CORNER       (1 << 2)
#define EDGE_HEIGHT_CORNER_FAR  (1 << 3)
#define EDGE_HEIGHT_CORNER_NEAR (1 << 4)
#define EDGE_HEIGHT_CORNER      (1 << 5)
#define EDGE_WIDTH_STRAIGHT     (1 << 6)
#define EDGE_WIDTH_BEND         (1 << 7)
#define EDGE_HEIGHT_STRAIGHT    (1 << 8)
#define EDGE_HEIGHT_BEND        (1 << 9)
#define EDGE_WIDTH_DRAW         (1 <<10)
#define EDGE_HEIGHT_DRAW        (1 <<11)
#define EDGE_WIDTH_PADDING      (1 <<12)
#define EDGE_HEIGHT_PADDING     (1 <<13)
#define EDGE_WIDTH_SKIP         (1 <<14)
#define EDGE_HEIGHT_SKIP        (1 <<15)

const int TILE_WIDTH = (3 + YABL_BOARD_WRITE_WIDTH);
const int WIDTH  = ( TILE_WIDTH ) * YABL_BOARD_WIDTH + 1;
const int TILE_HEIGHT= (1 + YABL_BOARD_WRITE_HEIGHT);
const int HEIGHT = ( TILE_HEIGHT ) * YABL_BOARD_HEIGHT + 1;
#define HEIGHT_PADDING 10

void printBoard()
{
	sigset_t set;
	int* set_ptr = (int*)&set;
	sigaddset(&set, SIGWINCH);

    ioctl(0, TIOCGWINSZ, &size);

	// CHECK FOR SIZE
    while(size.ws_col < WIDTH || size.ws_row < HEIGHT + HEIGHT_PADDING) {
        redraw_screen();
		fprintf(stdout, "\x1b[H\x1b[2J\x1b[3J");
		signal(SIGWINCH, &NULL_FUNC);
		pause();
		ioctl(0, TIOCGWINSZ, &size);
    } 

    fprintf(stdout, "\x1b[m\x1b[H\x1b[2J\x1b[3J");

	int drawStart = size.ws_col / 2 - WIDTH / 2;

	// DRAW THE BOARD
	printf("\x1b[?25l");
	for(int j = 0; j < HEIGHT; j++){
		printf("\x1b[E");
		for(int i = 0; i < WIDTH; i++){
			printf("\x1b[%dG", i + drawStart);
			char* p;
			int flags = 0;
			if      ( i == 0 )               flags |= EDGE_WIDTH_CORNER | EDGE_WIDTH_CORNER_NEAR;
			else if ( i == WIDTH -1 )        flags |= EDGE_WIDTH_CORNER | EDGE_WIDTH_CORNER_FAR;
			else if ( i % TILE_WIDTH == 0 )  flags |= EDGE_WIDTH_BEND;
			else                             flags |= EDGE_WIDTH_STRAIGHT;

			if      ( i % TILE_WIDTH == TILE_WIDTH / 2 )  flags |= EDGE_WIDTH_DRAW;

			if      ( j == 0 )               flags |= EDGE_HEIGHT_CORNER | EDGE_HEIGHT_CORNER_NEAR;
			else if ( j == HEIGHT - 1 )      flags |= EDGE_HEIGHT_CORNER | EDGE_HEIGHT_CORNER_FAR;
			else if ( j % TILE_HEIGHT == 0 ) flags |= EDGE_HEIGHT_BEND;
			else                             flags |= EDGE_HEIGHT_STRAIGHT;

			if ( j % TILE_HEIGHT == TILE_HEIGHT / 2 ) flags |= EDGE_HEIGHT_DRAW;

			if     (flags & EDGE_WIDTH_CORNER_FAR && flags & EDGE_HEIGHT_CORNER_FAR)
				printf("┘");
			else if(flags & EDGE_WIDTH_CORNER_NEAR && flags & EDGE_HEIGHT_CORNER_FAR)
				printf("└");
			else if(flags & EDGE_WIDTH_CORNER_NEAR && flags & EDGE_HEIGHT_CORNER_NEAR)
				printf("┌");
			else if(flags & EDGE_WIDTH_CORNER_FAR && flags & EDGE_HEIGHT_CORNER_NEAR)
				printf("┐");
			else if(flags & EDGE_WIDTH_BEND && flags & EDGE_HEIGHT_BEND)
				printf("┼");
			else if(flags & (EDGE_WIDTH_BEND | EDGE_WIDTH_CORNER_FAR | EDGE_WIDTH_CORNER_NEAR ) && flags & EDGE_HEIGHT_STRAIGHT )
				printf("│");
			else if(flags & EDGE_WIDTH_STRAIGHT && flags & (EDGE_HEIGHT_BEND | EDGE_HEIGHT_CORNER_FAR | EDGE_HEIGHT_CORNER_NEAR))
				printf("─");
			else if(flags & EDGE_WIDTH_CORNER_FAR && flags & EDGE_HEIGHT_BEND)
				printf("┤");
			else if(flags & EDGE_WIDTH_CORNER_NEAR && flags & EDGE_HEIGHT_BEND)
				printf("├");
			else if(flags & EDGE_WIDTH_BEND && flags & EDGE_HEIGHT_CORNER_FAR)
				printf("┴");
			else if(flags & EDGE_WIDTH_BEND && flags & EDGE_HEIGHT_CORNER_NEAR)
				printf("┬");
			else if( flags & EDGE_WIDTH_DRAW && flags & EDGE_HEIGHT_DRAW ){
				int x = i - YABL_BOARD_WRITE_WIDTH  / 2 - 1;
				x = x / (YABL_BOARD_WRITE_WIDTH + 3);
				int y = j - YABL_BOARD_WRITE_HEIGHT / 2;
				y = y / (YABL_BOARD_WRITE_HEIGHT + 1);
				String* s = gettoken(x, y);
				printf("%s", s->string);
				destroyString(s);
			}
		}
	}

	// DRAW THE DISPLACER
	printf("\x1b[E\x1b[0G\x1b[32m");
	for(int i = 0; i < size.ws_col; i++ ){
		putchar('*');
	}
	printf("\x1b[E\x1b[0G\x1b[m");
	printf("\x1b[?25h");
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

	if( (width < WIDTH) ){
		for(i = 0; i < height / 2; i++)
		{
			fprintf(stdout, "\x1b[1B");
			fflush(stdout);
		}

		fprintf(stdout, "\x1b[31m🡄 You need to increase the width of the terminal to accommodate the chosen board size.");
		fflush(stdout);
	}

    fprintf(stdout, "\x1b[H");
    fflush(stdout);

    for(i = 0; i < (width - 88) / 2; i++)
    {
        fprintf(stdout, "\x1b[1C");
        fflush(stdout);
    }

	if( (height < HEIGHT + HEIGHT_PADDING) ){
		fprintf(stdout, "\x1b[31m🡅 You need to increase the height of the terminal to accommodate the chosen board size.🡅");
		fflush(stdout);
	}
}



//#

#endif /* ifndef CODE_BOARD */
