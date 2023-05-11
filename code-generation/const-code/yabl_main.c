#ifndef CODE_MAIN
#define CODE_MAIN

#include "board.c"

void yablEventSetup();
void yablEventTurn();
void yablEventClose();
extern int GAME_RUNNING;
void __INITIATE_GLOABLS__();
//#
int main(){
	__INITIATE_GLOABLS__();
	yablEventSetup();
	do {
		printBoard();
		yablEventTurn();
	} while(GAME_RUNNING);
	yablEventClose();
}
//#
#endif

