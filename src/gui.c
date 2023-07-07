#include "cfour.h"
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_version.h>

void logsdlversion();


int init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    logsdlversion();

    return 0;
}

void logsdlversion() {
    SDL_version compiled;
    SDL_version linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    SDL_Log("Compiled against SDL version %u.%u.%u\n",
            compiled.major, compiled.minor, compiled.patch);
    SDL_Log("Linked against SDL version %u.%u.%u.\n",
            linked.major, linked.minor, linked.patch);
}

