#include "cfour.h"
#include <stdio.h>

void keyboardevent(SDL_Event event) {
    switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            quit();
    }
}
