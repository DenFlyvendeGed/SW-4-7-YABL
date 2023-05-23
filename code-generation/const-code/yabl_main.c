#ifndef CODE_MAIN
#define CODE_MAIN

#include "board.c"
#include "./events_initializers.c"

extern int GAME_RUNNING;
void __INITIATE_GLOABLS__();
void __INITIATE_TILES__();
extern char PLAYERS[3];
//#
int main(){
	__INITIATE_GLOABLS__();
	__INITIATE_TILES__();
	__EVENT_SETUP__();
	do {
		PLAYER_INDEX = (++PLAYER_INDEX) % (sizeof(PLAYERS)/sizeof(char*));
		printBoard();
		__EVENT_TURN__();
	} while(GAME_RUNNING);
	printBoard();
	__EVENT_CLOSE__();
}
//#
#endif

