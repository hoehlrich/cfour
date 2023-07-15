#include <SDL2/SDL.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_version.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>

#define NUMCOL  7
#define NUMROW  6
#define BYTE    8

int init();
void quit();
void renderboard(SDL_Renderer *renderer, SDL_Surface *screensurface, long bluemask, long redmask);
void update(SDL_Renderer *renderer, SDL_Surface *screensurface, long bluemask, long redmask);
int insidesection(SDL_Event event);
void keyboardevent(SDL_Event event);
long makemove(long *playermask, long fullmask, int col);
int choosemove(long ourmask, long opmask, int ourturn);

void printboard(long bluemask, long redmask);
int takeinput(long * playermask, long fullmask);
int checkwin(long playermask, long move);

