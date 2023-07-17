#include "cfour.h"
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <stdio.h>

long makeplayermove(long *playermaskp, long fullmask, int col);
long makeaimove(struct Board *board, int *scores);
void handlewin(struct Board board, int playerwon);

extern SDL_Renderer *renderer;
extern SDL_Surface *screensurface;
extern SDL_Window *window;
extern TTF_Font *font86;
extern int activesection;

int main() {
    init();
    SDL_Event event;

    struct Board board;
    board.aimask = 0;
    board.playermask = 0;

    int i;
    long move;
    int scores[NUMCOL];
    for (i = 0; i < NUMCOL; i++)
        scores[i] = 0;

    render(board);
    i = 0;
    while (SDL_WaitEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                cleanup();
                break;
            case SDL_KEYDOWN:
                keyboardevent(event, board);
                break;
            case SDL_MOUSEMOTION:
                activesection = insidesection(event);
                break;
            case SDL_MOUSEBUTTONDOWN:
                /* your turn */
                move = makeplayermove(&(board.playermask), board.aimask | board.playermask, activesection);
                if (move < 0)
                    break;
                if (checkwin(board.playermask, move))
                    handlewin(board, TRUE);
                render(board);
                i++;

                /* ais turn */
                move = makeaimove(&board, scores);
                if (move < 0)
                    break;
                drawscores(scores);
                if (checkwin(board.aimask, move))
                    handlewin(board, FALSE);
                i++;
        }
        update(board);
    }
}

void handlewin(struct Board board, int playerwon) {
    char *text;
    SDL_Surface *textsurface;
    textsurface = SDL_GetWindowSurface(window);
    if (playerwon)
        text = "You Won!";
    else
        text = "Game Over!";

    drawtext(textsurface, text, WIDTH/2, (HEIGHT/2) - SECTIONWIDTH, TRUE, font86);
    SDL_BlitSurface(textsurface, NULL, screensurface, NULL);
    render(board);
    SDL_Delay(3000);
    cleanup();
}

/* makeaimove: returns move on success; -1 on failure */
long makeaimove(struct Board *board, int *scores) {
    long move;
    int col;
    col = choosemove(*board, DEPTH, scores);
    move = makemove(&(board->aimask), fullmask(*board), col);
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
