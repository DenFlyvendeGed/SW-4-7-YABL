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
		PLAYER_INDEX = ++PLAYER_INDEX % (sizeof(PLAYERS)/sizeof(char*));

	} while(GAME_RUNNING);
	yablEventClose();
}
//#
#endif

