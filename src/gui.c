#include "cfour.h"
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>

#define FPS 30

void logsdlversion();

void renderboard(struct Board board);
void rendercircle(SDL_Renderer *renderer, int x0, int y0, int radius);
void renderbar(SDL_Renderer *renderer, int n);
void renderbars(SDL_Renderer *renderer);
void renderpieces(SDL_Renderer *renderer, struct Board board);

struct Coord idxtocoord(int xi, int yi);

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *screensurface;
SDL_Texture *texture;
TTF_Font *font16;
TTF_Font *font86;
int activesection;
int lastupdate;

struct Coord {
    int x;
    int y;
};

int init() {
    /* window, renderer and surface initialization */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    logsdlversion();

    window = SDL_CreateWindow("Connect Four", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    screensurface = SDL_GetWindowSurface(window);

    SDL_SetColorKey(screensurface, TRUE, 0);

    activesection = -1;
    
    /* font initialization */
    TTF_Init();
    font16 = TTF_OpenFont("/usr/share/fonts/liberation/LiberationMono-Regular.ttf", 16);
    font86 = TTF_OpenFont("/usr/share/fonts/liberation/LiberationMono-Regular.ttf", 86);

    return 0;
}

void cleanup() {
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(0);
}

void render(struct Board board) {
    /* clear render */
    lastupdate = SDL_GetTicks();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    /* render board directly onto renderer */
    renderboard(board);

    /* create texture from shared screensurface and copy onto renderer */
    texture = SDL_CreateTextureFromSurface(renderer, screensurface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    /* present render */
    SDL_RenderPresent(renderer);
}

void renderboard(struct Board board) {
    renderbars(renderer);
    renderpieces(renderer, board);
}

void update(struct Board board) {
    int ticks = SDL_GetTicks();
    if (ticks - lastupdate >= 1000 / FPS) {
        render(board);
    }
}

void drawscores(int *scores) {
    int i, x, y;
    char text[4];
    SDL_Rect dstrect = {WIDTH, 50, 0, 0};
    SDL_Surface *scoressurface;

    scoressurface = SDL_GetWindowSurface(window);
    SDL_FillRect(scoressurface, NULL, 0);
    SDL_BlitSurface(scoressurface, NULL, screensurface, &dstrect);

    x = SECTIONWIDTH/2;
    y = 0;
    for (i = 0; i < NUMCOL; i++) {
        snprintf(text, 4, "%d", scores[i]);
        drawtext(scoressurface, text, x, y, TRUE, font16);
        x += SECTIONWIDTH;
    }
}

void drawtext(SDL_Surface *surface, const char* text, int x, int y, int centered, TTF_Font *font) {
    SDL_Surface *textsurface;
    SDL_Rect dstrect;
    int w, h;

    SDL_Color white = { 0xFF, 0xFF, 0xFF, 0 };

    textsurface = TTF_RenderUTF8_Solid(font, text, white);

    TTF_SizeUTF8(font, text, &w, &h);
    dstrect.w = w;
    dstrect.h = h;
    dstrect.y = y;

    if (centered)
        dstrect.x = x - (w / 2);
    else
        dstrect.x = x;

    SDL_BlitSurface(textsurface, NULL, surface, &dstrect);
}

/* https://en.wikipedia.org/w/index.php?title=Midpoint_circle_algorithm&oldid=889172082#C_example */
void rendercircle(SDL_Renderer *renderer, int x0, int y0, int radius) {
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

void renderbar(SDL_Renderer *renderer, int n) {
    int radius, x, y0, y1, offset;

    radius = SECTIONWIDTH / 2;
    x = (SECTIONWIDTH / 2) + (SECTIONWIDTH * n);
    y0 = radius;
    y1 = SECTIONHEIGHT - radius;
    offset = SECTIONWIDTH / 20;

    rendercircle(renderer, x, y0, radius - offset);
    rendercircle(renderer, x, y1, radius - offset);

    for (x = (SECTIONWIDTH * n) + offset; x < (SECTIONWIDTH - offset) + (SECTIONWIDTH * n); x++) {
        SDL_RenderDrawLine(renderer, x, y0, x, y1);
    }
}

void renderbars(SDL_Renderer *renderer) {
    int i;
    for (i = 0; i < NUMCOL; i++) {
        SDL_SetRenderDrawColor(renderer, 2, 2, 2, SDL_ALPHA_OPAQUE);
        if (i == activesection) {
            SDL_SetRenderDrawColor(renderer, 9, 9, 9, SDL_ALPHA_OPAQUE);
        }
        renderbar(renderer, i);
    }
}

int insidesection(SDL_Event event) {
    return event.motion.x / SECTIONWIDTH;
}

void renderpieces(SDL_Renderer *renderer, struct Board board) {
    int i, j;
    long space;
    struct Coord coord;
    int radius = ((SECTIONWIDTH / 2) * 4) / 5;

    for (i = 0; i < NUMROW; i++) {
        for (j = 0; j < NUMCOL; j++) {
            space = pow(2, (BYTE*i) + j);
            coord = idxtocoord(j, i);
            if (board.aimask & space) {
                SDL_SetRenderDrawColor(renderer, 32, 62, 160, SDL_ALPHA_OPAQUE);
                rendercircle(renderer, coord.x, coord.y, radius);
            } else if (board.playermask & space) {
                SDL_SetRenderDrawColor(renderer, 170, 20, 20, SDL_ALPHA_OPAQUE);
                rendercircle(renderer, coord.x, coord.y, radius);
            }
        }
    }
}

struct Coord idxtocoord(int xi, int yi) {
    struct Coord coord;
    coord.x = (SECTIONWIDTH / 2) + (xi * SECTIONWIDTH);
    coord.y = SECTIONHEIGHT - (SECTIONWIDTH / 2) - (yi * SECTIONWIDTH);
    return coord;
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

