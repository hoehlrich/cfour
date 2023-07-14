#include "cfour.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    init();
    extern SDL_Window *window;
    extern SDL_Renderer *renderer;
    extern SDL_Surface *screensurface;
    SDL_Texture *texture;

    renderboard(renderer, screensurface, texture);

    long bluemask, redmask;
    bluemask = redmask = 0;

    int i;
    long * playermaskp;
    long move;
    for (i = 0; 1; i++) {
        CLS;
        printboard(bluemask, redmask);
        playermaskp = i % 2 ? &bluemask : &redmask;
        move = takeinput(playermaskp, bluemask | redmask);
        if (checkwin(*playermaskp, move)) {
            CLS;
            char winner;
            winner = i % 2 ? 'X' : 'O';
            printboard(bluemask, redmask);
            printf("%c won!\n", winner);
            quit();
        }
    }
}

