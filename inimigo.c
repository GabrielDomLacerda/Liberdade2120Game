#include "header/geometria.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct inimigo {
    SDL_Rect posicao;
    SDL_Rect sprite;
    int vida, nivel;
    SDL_RendererFlip virado;
    int n_sprites;
    int bateu;
} INIMIGO;

typedef struct inimigos{
    INIMIGO *lista;
    int size;
    int max;
} COLECAO_INIMIGOS;

//face = 1: esquerda, face = 0:direita

INIMIGO criar_inimigo(int x, int nivel, int face, int w, int h) {
    INIMIGO inimigo;
    inimigo.vida = nivel+1;
    inimigo.nivel = nivel;
    inimigo.bateu = 0;
    if (nivel == 2) {
        inimigo.n_sprites = 6;
    }
    else {
        inimigo.n_sprites = 3;
    }
    setrect(&(inimigo.posicao), x, ALTURA_PERSONAGEM+50, w, h);
    if (face) {
        inimigo.virado = SDL_FLIP_HORIZONTAL;
    }
    else {
        inimigo.virado = SDL_FLIP_NONE;
    }
    return inimigo;
}

COLECAO_INIMIGOS *criar_lista_inimigos(int max) {
    COLECAO_INIMIGOS *colecao;
    colecao = (COLECAO_INIMIGOS *) malloc(sizeof(COLECAO_INIMIGOS));
    colecao->size = 0;
    colecao->max = max;
    colecao->lista = (INIMIGO *) malloc(sizeof(INIMIGO) * max);
}

int size_inimigos(COLECAO_INIMIGOS *colecao) {
    return colecao->size;
}

int max_inimigos(COLECAO_INIMIGOS *colecao) {
    return colecao->max;
}

void insere_inimigo(COLECAO_INIMIGOS *colecao, int x, int nivel, int face, int w, int h) {
    if (size_inimigos(colecao) >= max_inimigos(colecao)) {
        return;
    }
    INIMIGO p;
    p = criar_inimigo(x, nivel, face, w, h);
    colecao->lista[size_inimigos(colecao)] = p;
    colecao->size++;
}

void remove_inimigo(COLECAO_INIMIGOS *colecao, int idx) {
    if (size_inimigos(colecao) <= 0 || idx >= size_inimigos(colecao)) {
        return;
    }
    colecao->lista[idx] = colecao->lista[size_inimigos(colecao)-1];
    colecao->size--;
}

INIMIGO* acessa_inimigo(COLECAO_INIMIGOS *colecao, int idx) {
    return &(colecao->lista[idx]); 
}

void mostrar_1(INIMIGO *inimigo) {
    printf("\tVida: %d\n", inimigo->vida);
    printf("\tdx: %d|dy: %d\n", inimigo->posicao.x, inimigo->posicao.y);
}

void mostrar_inimigos(COLECAO_INIMIGOS *colecao) {
    int i;
    printf("------------\n");
    printf("Estado atual:\n");
    printf("\tTamanho: %d/%d\n", size_inimigos(colecao), max_inimigos(colecao));
    for (i = 0; i < size_inimigos(colecao); i++) {
        printf("Inimigo [%d]: \n", i+1);
        mostrar_1(acessa_inimigo(colecao, i));
    }
}

int w_inimigo(COLECAO_INIMIGOS *colecao, int idx) {
    return acessa_inimigo(colecao, idx)->posicao.w;
}

int h_inimigo(COLECAO_INIMIGOS *colecao, int idx) {
    return acessa_inimigo(colecao, idx)->posicao.w;
}

SDL_Rect *sprite_inimigo(COLECAO_INIMIGOS *colecao, int idx) {
    return &(acessa_inimigo(colecao, idx)->sprite);
}

SDL_Rect *pos_inimigo(COLECAO_INIMIGOS *colecao, int idx) {
    return &(acessa_inimigo(colecao, idx)->posicao);
}

SDL_RendererFlip face_inimigo(COLECAO_INIMIGOS *colecao, int idx) {
    return (acessa_inimigo(colecao, idx)->virado);
}

int andar_inimigo(COLECAO_INIMIGOS *colecao, int idx, int velocidade) {
    if (face_inimigo(colecao, idx) == SDL_FLIP_HORIZONTAL) {
        if (pos_inimigo(colecao, idx)->x >= MAX_POS(face_inimigo(colecao, idx))) {
            pos_inimigo(colecao, idx)->x -= velocidade;
            return 1;
        }
    }
    else {
        if (pos_inimigo(colecao, idx)->x <= MAX_POS(face_inimigo(colecao, idx))) {
            pos_inimigo(colecao, idx)->x += velocidade;
            return 1;
        }  
    }   
    return 0;
}

void vira_inimigo(COLECAO_INIMIGOS *colecao, int idx) {
    if (face_inimigo(colecao, idx) == SDL_FLIP_HORIZONTAL) {
        acessa_inimigo(colecao, idx)->virado = SDL_FLIP_NONE;
    }
    else {
        acessa_inimigo(colecao, idx)->virado = SDL_FLIP_HORIZONTAL;
    }
}

void remove_ultimo_inimigo(COLECAO_INIMIGOS *pcolecao) {
    remove_inimigo(pcolecao, size_inimigos(pcolecao) - 1);
}

int* vida_inimigo(COLECAO_INIMIGOS* pcolecao, int idx) {
    return &(acessa_inimigo(pcolecao, idx)->vida);
}

int nivel_inimigo(COLECAO_INIMIGOS* pcolecao, int idx) {
    return acessa_inimigo(pcolecao, idx)->nivel;
}

int ultimo_adicionado(COLECAO_INIMIGOS* pcolecao) {
    return size_inimigos(pcolecao) - 1;
}

int n_sprites(COLECAO_INIMIGOS* pcolecao, int idx) {
    return acessa_inimigo(pcolecao, idx)->n_sprites;
}

void muda_sprite(COLECAO_INIMIGOS* pcolecao, int idx, int count) {
    sprite_inimigo(pcolecao, idx)->x = sprite_inimigo(pcolecao, idx)->w * (count % n_sprites(pcolecao, idx));
}

void muda_imagem(COLECAO_INIMIGOS* pcolecao, int idx, SDL_Rect rect, int quant_sprites) {
    setrect_direto(sprite_inimigo(pcolecao, idx), rect);
    acessa_inimigo(pcolecao, idx)->n_sprites = quant_sprites;
}

int* bateu(COLECAO_INIMIGOS* pcolecao, int idx) {
    return &(acessa_inimigo(pcolecao, idx)->bateu);
}