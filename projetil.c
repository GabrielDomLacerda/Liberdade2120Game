#include <SDL2/SDL.h>
#include "header/geometria.h"
#define MAX_PROJETEIS 20

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

CONJUNTO_PROJETEIS *gera_projeteis() {
    CONJUNTO_PROJETEIS *conjunto;
    conjunto = (CONJUNTO_PROJETEIS *) malloc(sizeof(CONJUNTO_PROJETEIS));
    conjunto->size = 0;
    conjunto->max = MAX_PROJETEIS;
    conjunto->lista_tiros = (PROJETIL *) malloc(sizeof(PROJETIL) * MAX_PROJETEIS);
}

PROJETIL gera_tiro(SDL_Rect posicao, SDL_Rect sprite, int face) {
    PROJETIL novo;
    if (face) {
        novo.face = SDL_FLIP_HORIZONTAL;
    }
    else {
        novo.face = SDL_FLIP_NONE;
    }
    setrect_direto(&(novo.posicao), posicao);
    setrect_direto(&(novo.sprite), sprite);
    return novo;
}

int quant_tiros(CONJUNTO_PROJETEIS* conjunto) {
    return conjunto->size;
}

PROJETIL* acessa_projetil(CONJUNTO_PROJETEIS *pconjunto, int idx) {
    return &(pconjunto->lista_tiros[idx]);
}

void surge_projetil(CONJUNTO_PROJETEIS *conjunto, SDL_Rect posicao, SDL_Rect sprite, int face) {
    if (quant_tiros(conjunto) >= MAX_PROJETEIS) {
        return;
    }
    PROJETIL novo;
    novo = gera_tiro(posicao, sprite, face);
    conjunto->lista_tiros[quant_tiros(conjunto)] = novo;
    conjunto->size++;
}


SDL_Rect* sprite_tiro(CONJUNTO_PROJETEIS *pconjunto, int idx) {
    return &(acessa_projetil(pconjunto, idx)->sprite);
}

SDL_Rect* pos_tiro(CONJUNTO_PROJETEIS *pconjunto, int idx) {
    return &(acessa_projetil(pconjunto, idx)->posicao);
}

SDL_RendererFlip face_tiro(CONJUNTO_PROJETEIS *pconjunto, int idx) {
    return acessa_projetil(pconjunto, idx)->face;
}


void remove_tiro(CONJUNTO_PROJETEIS *pconjunto, int idx) {
    if (quant_tiros(pconjunto) < 1 || idx >= quant_tiros(pconjunto)) {
        return;
    }
    pconjunto->lista_tiros[idx] = pconjunto->lista_tiros[quant_tiros(pconjunto) - 1];
    pconjunto->size--;
}

int move_tiro(CONJUNTO_PROJETEIS *pconjunto, int idx) {
    int velocidade = 8;
    if (face_tiro(pconjunto, idx) == SDL_FLIP_HORIZONTAL) {
        if (pos_tiro(pconjunto, idx)->x > MIN_POS(SDL_FLIP_NONE)) {
            pos_tiro(pconjunto, idx)->x -= velocidade;
            return 1;
        }
    }
    else {
        if (pos_tiro(pconjunto, idx)->x < MIN_POS(SDL_FLIP_HORIZONTAL)) {
            pos_tiro(pconjunto, idx)->x += velocidade;
            return 1;
        }
    }
    return 0;
}