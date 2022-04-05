#define ALTURA_PERSONAGEM 400
#include <SDL2/SDL.h>

int distancia(int pos1, int pos2);
void setrect(SDL_Rect *spr, int x1, int y1, int w1, int h1);
void setrect_direto(SDL_Rect *spr, SDL_Rect aux);
int MAX_POS(SDL_RendererFlip face);
int MIN_POS(SDL_RendererFlip face);