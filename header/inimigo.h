#include "geometria.h"

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
INIMIGO criar_inimigo(int x, int nivel, int face, int w, int h);
COLECAO_INIMIGOS *criar_lista_inimigos(int max);
int size_inimigos(COLECAO_INIMIGOS *colecao);
int max_inimigos(COLECAO_INIMIGOS *colecao);
void insere_inimigo(COLECAO_INIMIGOS *colecao, int x, int nivel, int face, int w, int h);
void remove_inimigo(COLECAO_INIMIGOS *colecao, int idx);
INIMIGO* acessa_inimigo(COLECAO_INIMIGOS *colecao, int idx);
void mostrar_1(INIMIGO *inimigo);
void mostrar_inimigos(COLECAO_INIMIGOS *colecao);
int w_inimigo(COLECAO_INIMIGOS *colecao, int idx);
int h_inimigo(COLECAO_INIMIGOS *colecao, int idx);
SDL_Rect *sprite_inimigo(COLECAO_INIMIGOS *colecao, int idx);
SDL_Rect *pos_inimigo(COLECAO_INIMIGOS *colecao, int idx);
SDL_RendererFlip face_inimigo(COLECAO_INIMIGOS *colecao, int idx);
int andar_inimigo(COLECAO_INIMIGOS *colecao, int idx, int velocidade);
void vira_inimigo(COLECAO_INIMIGOS *colecao, int idx);
void remove_ultimo_inimigo(COLECAO_INIMIGOS *pcolecao);
int* vida_inimigo(COLECAO_INIMIGOS* pcolecao, int idx);
int nivel_inimigo(COLECAO_INIMIGOS* pcolecao, int idx);
int ultimo_adicionado(COLECAO_INIMIGOS* pcolecao);
int n_sprites(COLECAO_INIMIGOS* pcolecao, int idx);
void muda_sprite(COLECAO_INIMIGOS* pcolecao, int idx, int count) ;
void muda_imagem(COLECAO_INIMIGOS* pcolecao, int idx, SDL_Rect rect, int quant_sprites);
int* bateu(COLECAO_INIMIGOS* pcolecao, int idx);