#include "cfour.h"
#include <stdio.h>

void keyboardevent(SDL_Event event, struct Board board) {
    switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            cleanup();
            break;
        case SDLK_h:
            printf("heuristic: %d\n", heuristic(board));
            break;
    }
}
