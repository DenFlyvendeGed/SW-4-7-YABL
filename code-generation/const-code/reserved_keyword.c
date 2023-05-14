#ifndef CODE_RESERVED_KEYWORDS
#define CODE_RESERVED_KEYWORDS
#include "globals.c"
extern char** PLAYERS;

//#

void endgame(){
    GAME_RUNNING = 0;
}

__STRING__T* currentPlayer(){
    return __MAKE_STRING__(PLAYERS[PLAYER_INDEX]);
}


//#

#endif
