#include "cfour.h"
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    init();
    extern SDL_Renderer *renderer;
    extern SDL_Surface *screensurface;
    SDL_Event event;
    extern int activesection;

    long bluemask, redmask;
    bluemask = redmask = 0;

    int i;
    long *playermaskp;
    long move;
    renderboard(renderer, screensurface, bluemask, redmask);
    i = 0;
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
                if (move == -1)
                    break;
                if (checkwin(*playermaskp, move)) {
                    char *winner;
                    winner = i % 2 ? "Blue" : "Red";
                    printf("%s won!\n", winner);
                    renderboard(renderer, screensurface, bluemask, redmask);
                    SDL_Delay(3000);
                    quit();
                }
                i++;
        }
        update(renderer, screensurface, bluemask, redmask);
    }
}

