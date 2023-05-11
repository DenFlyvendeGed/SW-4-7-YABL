#ifndef CODE_RESERVED_KEYWORDS
#define CODE_RESERVED_KEYWORDS
#include "globals.c"
extern char** PLAYERS;

//#

void endgame(){
    GAME_RUNNING = 0;
}

String* currentPlayer(){
    return makeString(PLAYERS[PLAYER_INDEX]);
}


//#

#endif