#include <SDL2/SDL.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_version.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_ttf.h>

#define TRUE    1
#define FALSE   0
#define NUMCOL  7
#define NUMROW  6
#define BYTE    8
#define DEPTH   6

#define ASP_WIDTH   7
#define ASP_HEIGHT  6
#define SCALE   150
#define WIDTH   (ASP_WIDTH*SCALE)
#define HEIGHT  (ASP_HEIGHT*SCALE)
#define SECTIONWIDTH    (WIDTH / NUMCOL)
#define SECTIONHEIGHT   HEIGHT

struct Board {
    long playermask;
    long aimask;
};

int init();
void cleanup();
void render(struct Board board);
void update(struct Board board);
int insidesection(SDL_Event event);
void keyboardevent(SDL_Event event, struct Board board);
void drawscores(int * scores);
void drawtext(SDL_Surface *surface, const char* text, int x, int y, int centered, TTF_Font *font);

long makemove(long *mask, long fullmask, int col);
int choosemove(struct Board board, int depth, int *scores);
long fullmask(struct Board board);
int heuristic(struct Board board);

void printboard(struct Board board);
int checkwin(long playermask, long move);

