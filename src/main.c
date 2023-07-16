#include "cfour.h"
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <stdio.h>

long makeplayermove(long *playermaskp, long fullmask, int col);
long makeaimove(long *aimaskp, long fullmask);

int main() {
    init();
    extern SDL_Renderer *renderer;
    extern SDL_Surface *screensurface;
    SDL_Event event;
    extern int activesection;

    struct Board board;
    board.aimask = 0;
    board.playermask = 0;

    int i;
    long move;
    renderboard(renderer, screensurface, board);
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
                /* your turn */
                move = makeplayermove(&(board.playermask), board.aimask | board.playermask, activesection);
                if (move < 0)
                    break;
                if (checkwin(board.playermask, move)) {
                    printf("You won!\n");
                    renderboard(renderer, screensurface, board);
                    SDL_Delay(3000);
                    quit();
                }
                renderboard(renderer, screensurface, board);
                i++;

                /* ais turn */
                move = makeaimove(&(board.aimask), board.aimask | board.playermask);
                if (move < 0)
                    break;
                if (checkwin(board.aimask, move)) {
                    printf("You lost!\n");
                    renderboard(renderer, screensurface, board);
                    SDL_Delay(3000);
                    quit();
                }
            i++;
        }
        update(renderer, screensurface, board);
    }
}

/* makeaimove: returns move on success; -1 on failure */
long makeaimove(long *aimaskp, long fullmask) {
    long move;
    int col;
    col = choosemove(*aimaskp, fullmask ^ (*aimaskp), 6);
    move = makemove(aimaskp, fullmask, col);
    if (move == -1)
        return -1;
    return move;
}

/* makeplayermove: returns move on success; -1 on failure */
long makeplayermove(long *playermaskp, long fullmask, int col) {
    long move;
    move = makemove(playermaskp, fullmask, col);
    if (move == -1)
        return -1;
    return move;
}
