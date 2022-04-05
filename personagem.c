#include <SDL2/SDL.h>
#include "header/geometria.h"

typedef struct {
    SDL_Rect sprite_personagem;
    SDL_Rect pos_personagem;
    int vida, estado;
    SDL_Rect heart[3];
    SDL_RendererFlip face;
} PERSONAGEM;

int andar(PERSONAGEM *boneco, int* direita, int* esquerda) {
    if (direita) {
            boneco->pos_personagem.x += 8;
            return 0;
    }
    if (esquerda) {
            boneco->pos_personagem.x -= 8;
            return 1;
    }
}



