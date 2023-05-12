#ifndef CODE_MAIN
#define CODE_MAIN

#include "board.c"

void yablEventSetup();
void yablEventTurn();
void yablEventClose();
extern int GAME_RUNNING;
void __INITIATE_GLOABLS__();
void __INITIATE_TILES__();
extern char PLAYERS[3];
//#
int main(){
	__INITIATE_GLOABLS__();
	__INITIATE_TILES__();
	yablEventSetup();
	do {
		printBoard();
		yablEventTurn();
		PLAYER_INDEX = (++PLAYER_INDEX) % (sizeof(PLAYERS)/sizeof(char*));
	} while(GAME_RUNNING);
	yablEventClose();
}
//#
#endif

