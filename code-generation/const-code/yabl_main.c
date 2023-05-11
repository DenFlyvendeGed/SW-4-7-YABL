#ifndef CODE_MAIN
#define CODE_MAIN

#include "board.c"

void yablEventSetup();
void yablEventTurn();
void yablEventClose();
extern int GAME_RUNNING;
//#
int main(){
	yablEventSetup();
	do {
		printBoard();
		yablEventTurn();
	} while(GAME_RUNNING);
	yablEventClose();
}
//#
#endif

