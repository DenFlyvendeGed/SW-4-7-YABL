#include <asm-generic/ioctls.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>

struct winsize sz;

void redraw(){
        printf( "\x1b[H\x1b[2J\x1b[3J" );
        ioctl( 0, TIOCGWINSZ, &sz );
        int limit = sz.ws_row * sz.ws_col;
        for( int i = 0; i < limit; i++ ){
                fprintf(stdout, "X");
        }
}

int main( int argc, char **argv ){
        redraw();
        int j;
        while(j != 0)
        {
                signal( SIGWINCH, redraw );
        }
        return 0;
}