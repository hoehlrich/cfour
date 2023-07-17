#include "cfour.h"
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

#define FPS 30
#define FONTPT  16
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
void drawtext(SDL_Renderer *renderer, const char* text, int x, int y, int centered);
void drawpieces(SDL_Renderer *renderer, struct Board board);
void drawscores(SDL_Renderer *renderer, int * scores);
struct Coord idxtocoord(int xi, int yi);

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *screensurface;
SDL_Surface *text;
TTF_Font *font;
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

    activesection = -1;
    
    /* font initialization */
    TTF_Init();
    font = TTF_OpenFont("/usr/share/fonts/liberation/LiberationMono-Regular.ttf", FONTPT);
    if (font == NULL)
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to load font\n");

    return 0;
}

void cleanup() {
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(0);
}

void render(SDL_Renderer *renderer, SDL_Surface *screensurface, struct Board board, int *scores) {
    lastupdate = SDL_GetTicks();
    SDL_RenderClear(renderer);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, screensurface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    drawbars(renderer);
    drawpieces(renderer, board);
    drawscores(renderer, scores);

    SDL_RenderPresent(renderer);
}

void update(SDL_Renderer *renderer, SDL_Surface *screensurface, struct Board board, int *scores) {
    int ticks = SDL_GetTicks();
    if (ticks - lastupdate >= 1000 / FPS) {
        render(renderer, screensurface, board, scores);
    }
}

void drawscores(SDL_Renderer *renderer, int *scores) {
    int i, x, y;
    char text[4];

    x = SECTIONWIDTH/2;
    y = 0;
    for (i = 0; i < NUMCOL; i++) {
        snprintf(text, 4, "%d", scores[i]);
        drawtext(renderer, text, x, y, TRUE);
        x += SECTIONWIDTH;
    }
}

void drawtext(SDL_Renderer *renderer, const char* text, int x, int y, int centered) {
    SDL_Texture *texture;
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

    texture = SDL_CreateTextureFromSurface(renderer, textsurface);
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
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
    offset = SECTIONWIDTH / 20;

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

void drawpieces(SDL_Renderer *renderer, struct Board board) {
    int i, j;
    long space;
    struct Coord coord;
    int radius = (SECTIONWIDTH / 2) * 0.8;

    for (i = 0; i < NUMROW; i++) {
        for (j = 0; j < NUMCOL; j++) {
            space = pow(2, (BYTE*i) + j);
            coord = idxtocoord(j, i);
            if (board.aimask & space) {
                SDL_SetRenderDrawColor(renderer, 32, 62, 160, SDL_ALPHA_OPAQUE);
                drawcircle(renderer, coord.x, coord.y, radius);
            } else if (board.playermask & space) {
                SDL_SetRenderDrawColor(renderer, 170, 20, 20, SDL_ALPHA_OPAQUE);
                drawcircle(renderer, coord.x, coord.y, radius);
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

