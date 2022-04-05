#include <SDL2/SDL.h>
#include <math.h>

int distancia(int pos1, int pos2) {
    return pos1 - pos2;
}

void setrect(SDL_Rect *spr, int x1, int y1, int w1, int h1) {
    spr->x = x1;
    spr->y = y1;
    spr->w = w1;
    spr->h = h1;
}

void setrect_direto(SDL_Rect *spr, SDL_Rect aux) {
    setrect(spr, aux.x, aux.y, aux.w, aux.h);
}

int MAX_POS(SDL_RendererFlip face) {
    if (face) {
        return 400;
    }
    return 260;
}

int MIN_POS(SDL_RendererFlip face) {
    if (face) {
        return 800;
    }
    return -100;
}
