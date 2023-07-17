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
#define DEPTH   7

struct Board {
    long playermask;
    long aimask;
};

int init();
void cleanup();
void render(SDL_Renderer *renderer, SDL_Surface *screensurface, struct Board board, int *scores);
void update(SDL_Renderer *renderer, SDL_Surface *screensurface, struct Board board, int *scores);
int insidesection(SDL_Event event);
void keyboardevent(SDL_Event event, struct Board board);
long makemove(long *mask, long fullmask, int col);
int choosemove(struct Board board, int depth, int *scores);
int getscore(long ourmask, long opmask, int depth, int ourmove);
long fullmask(struct Board board);
int heuristic(struct Board board);

void printboard(struct Board board);
int takeinput(long * playermask, long fullmask);
int checkwin(long playermask, long move);

