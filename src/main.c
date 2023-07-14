#include "cfour.h"
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    init();
    extern SDL_Window *window;
    extern SDL_Renderer *renderer;
    extern SDL_Surface *screensurface;
    SDL_Texture *texture;
    SDL_Event event;
    extern int activesection;

    int lastupdate, ticks;

    long bluemask, redmask;
    bluemask = redmask = 0;

    int i = 0;
    long *playermaskp;
    long move;
    renderboard(renderer, screensurface, texture, bluemask, redmask);
    while (SDL_WaitEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit();
                break;
            case SDL_KEYDOWN:
                keyboardevent(event);
                break;
            case SDL_MOUSEMOTION:
                activesection = insidesection(event);
                break;
            case SDL_MOUSEBUTTONDOWN:
                playermaskp = i % 2 ? &bluemask : &redmask;
                move = makemove(playermaskp, bluemask | redmask, activesection);
                if (checkwin(*playermaskp, move)) {
                    char winner;
                    winner = i % 2 ? 'O' : 'X';
                    printf("%c won!\n", winner);
                    renderboard(renderer, screensurface, texture, bluemask, redmask);
                    SDL_Delay(3000);
                    quit();
                }
                i++;
                break;
        }
        update(renderer, screensurface, texture, bluemask, redmask);
    }





    for (i = 0; 1; i++) {
        CLS;
        printboard(bluemask, redmask);
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

