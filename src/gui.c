#include "cfour.h"

#define ASP_WIDTH   7
#define ASP_HEIGHT  6
#define SCALE       50
#define WIDTH       ASP_WIDTH*SCALE
#define HEIGHT      ASP_HEIGHT*SCALE

void logsdlversion();

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *screensurface;

int init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    logsdlversion();

    window = SDL_CreateWindow("Connect Four", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (window == NULL)
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window could not be created! SDL Error: %s\n", SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, 0);

    screensurface = SDL_GetWindowSurface(window);

    /* draw background */
    Uint32 black = SDL_MapRGB(screensurface->format, 0, 0, 0);
    SDL_FillRect(screensurface, NULL, black);

    return 0;
}

void quit() {
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(0);
}

void renderboard(SDL_Renderer *renderer, SDL_Surface *screensurface, SDL_Texture *texture) {
    SDL_RenderClear(renderer);
    texture = SDL_CreateTextureFromSurface(renderer, screensurface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
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

