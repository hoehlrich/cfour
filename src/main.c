#include <stdlib.h>
#include <stdio.h>
#include "cfour.h"

int main() {
    init();
    long bluemask, redmask;
    bluemask = redmask = 0;

    int i;
    long * playermaskp;
    long move;
    for (i = 0; 1; i++) {
        printboard(bluemask, redmask);
        playermaskp = i % 2 ? &bluemask : &redmask;
        move = takeinput(playermaskp, bluemask | redmask);
        if (checkwin(*playermaskp, move)) {
            char winner;
            winner = i % 2 ? 'X' : 'O';
            printboard(bluemask, redmask);
            printf("%c won!\n", winner);
            exit(0);
        }
    }
}

