#include <SDL2/SDL.h>
#define MAX_PROJETEIS 2

typedef struct tiro {
    SDL_Rect posicao;
    SDL_Rect sprite;
    SDL_RendererFlip face;
} PROJETIL;

typedef struct tiros {
    PROJETIL *lista_tiros;
    int size;
    int max;
} CONJUNTO_PROJETEIS;

CONJUNTO_PROJETEIS *gera_projeteis();
PROJETIL gera_tiro(SDL_Rect posicao, SDL_Rect sprite, int face);
int quant_tiros(CONJUNTO_PROJETEIS* conjunto);
PROJETIL* acessa_projetil(CONJUNTO_PROJETEIS *pconjunto, int idx) ;
void surge_projetil(CONJUNTO_PROJETEIS *conjunto, SDL_Rect posicao, SDL_Rect sprite, int face);
SDL_Rect* sprite_tiro(CONJUNTO_PROJETEIS *pconjunto, int idx);
SDL_Rect* pos_tiro(CONJUNTO_PROJETEIS *pconjunto, int idx);
SDL_RendererFlip face_tiro(CONJUNTO_PROJETEIS *pconjunto, int idx);
void remove_tiro(CONJUNTO_PROJETEIS *pconjunto, int idx);
int move_tiro(CONJUNTO_PROJETEIS *pconjunto, int idx);