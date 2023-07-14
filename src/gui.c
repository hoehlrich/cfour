#include "cfour.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>

#define FPS 30
#define ASP_WIDTH   7
#define ASP_HEIGHT  6
#define SCALE   150
#define WIDTH   (ASP_WIDTH*SCALE)
#define HEIGHT  (ASP_HEIGHT*SCALE)
#define SECTIONWIDTH    (WIDTH / NUMCOL)
#define SECTIONHEIGHT   HEIGHT

void logsdlversion();
void drawcircle(SDL_Renderer *renderer, int x0, int y0, int radius);
void drawbar(SDL_Renderer *renderer, int n);
void drawbars(SDL_Renderer *renderer);
void drawpieces(SDL_Renderer *renderer, long bluemask, long redmask);

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *screensurface;
int activesection;
int lastupdate;

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
    activesection = -1;

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

void renderboard(SDL_Renderer *renderer, SDL_Surface *screensurface, SDL_Texture *texture, long bluemask, long redmask) {
    lastupdate = SDL_GetTicks();
    SDL_RenderClear(renderer);
    texture = SDL_CreateTextureFromSurface(renderer, screensurface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    drawbars(renderer);
    drawpieces(renderer, bluemask, redmask);

    SDL_RenderPresent(renderer);
}

void update(SDL_Renderer *renderer, SDL_Surface *screensurface, SDL_Texture *texture, long bluemask, long redmask) {
    int ticks = SDL_GetTicks();
    if (ticks - lastupdate >= 1000 / FPS) {
        renderboard(renderer, screensurface, texture, bluemask, redmask);
    }
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

/* https://en.wikipedia.org/w/index.php?title=Midpoint_circle_algorithm&oldid=889172082#C_example */
void drawcircle(SDL_Renderer *renderer, int x0, int y0, int radius) {
    int x = radius -1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        SDL_RenderDrawLine(renderer, x0 + y, y0 + x, x0 - y, y0 + x);
        SDL_RenderDrawLine(renderer, x0 + x, y0 + y, x0 - x, y0 + y);
        SDL_RenderDrawLine(renderer, x0 - y, y0 - x, x0 + y, y0 - x);
        SDL_RenderDrawLine(renderer, x0 - x, y0 - y, x0 + x, y0 - y);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void drawbar(SDL_Renderer *renderer, int n) {
    int radius, x, y0, y1, offset;

    radius = SECTIONWIDTH / 2;
    x = (SECTIONWIDTH / 2) + (SECTIONWIDTH * n);
    y0 = radius;
    y1 = SECTIONHEIGHT - radius;
    offset = SECTIONWIDTH * 0.05;

    drawcircle(renderer, x, y0, radius - offset);
    drawcircle(renderer, x, y1, radius - offset);

    for (x = (SECTIONWIDTH * n) + offset; x < (SECTIONWIDTH - offset) + (SECTIONWIDTH * n); x++) {
        SDL_RenderDrawLine(renderer, x, y0, x, y1);
    }
}

void drawbars(SDL_Renderer *renderer) {
    int i;
    for (i = 0; i < NUMCOL; i++) {
        SDL_SetRenderDrawColor(renderer, 2, 2, 2, SDL_ALPHA_OPAQUE);
        if (i == activesection) {
            SDL_SetRenderDrawColor(renderer, 9, 9, 9, SDL_ALPHA_OPAQUE);
        }
        drawbar(renderer, i);
    }
}

int insidesection(SDL_Event event) {
    return event.motion.x / SECTIONWIDTH;
}

void drawpieces(SDL_Renderer *renderer, long bluemask, long redmask) {
    int i, j, x, y;
    long space;
    int radius = (SECTIONWIDTH / 2) * 0.8;

    x = SECTIONWIDTH / 2;
    y = SECTIONHEIGHT - (SECTIONWIDTH / 2);

    for (i = 0; i < NUMROW; i++) {
        for (j = 0; j < NUMCOL; j++) {
            space = pow(2, (BYTE*i) + j);
            if (bluemask & space) {
                SDL_SetRenderDrawColor(renderer, 32, 62, 160, SDL_ALPHA_OPAQUE);
                drawcircle(renderer, x, y, radius);
            } else if (redmask & space) {
                SDL_SetRenderDrawColor(renderer, 170, 20, 20, SDL_ALPHA_OPAQUE);
                drawcircle(renderer, x, y, radius);
            }
            x += SECTIONWIDTH;
        }
        y -= SECTIONWIDTH;
        x = SECTIONWIDTH / 2;
    }
}
