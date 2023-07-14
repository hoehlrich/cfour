#include <SDL2/SDL.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_version.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

#define CLS printf("\e[1;1H\e[2J");

int init();
void quit();
void printboard(long bluemask, long redmask);
int takeinput(long * playermask, long fullmask);
int checkwin(long playermask, long move);
void renderboard(SDL_Renderer *renderer, SDL_Surface *screensurface, SDL_Texture *texture);

