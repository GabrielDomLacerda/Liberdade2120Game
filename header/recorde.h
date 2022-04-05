#define MAX_RECORDES 50

typedef struct recorde {
    unsigned long int pontos;
    char nome[4];
} RECORDE;

typedef struct lista_recordes {
    RECORDE recordes[MAX_RECORDES];
    int size;
} LISTA_RECORDES;

LISTA_RECORDES inicia_lista();
unsigned int size_recordes(LISTA_RECORDES lista);
RECORDE* acessa_recorde(LISTA_RECORDES *lista, int idx);
void adiciona_recorde(LISTA_RECORDES* lista, unsigned long pontuacao, char letras[]);
void arquivar_recordes(LISTA_RECORDES lista);
void ler_recordes(LISTA_RECORDES *plista);
int pontos(LISTA_RECORDES *lista, int idx);
void troca(RECORDE *a, RECORDE *b);
void ordenar_recordes (LISTA_RECORDES *plista);
void transforma_recorde(int x, char resp[]);
void maiuscula(char string[]);
void preencher_lista(LISTA_RECORDES* lista);