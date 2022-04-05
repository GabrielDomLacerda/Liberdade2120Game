#include <stdio.h>
#include <string.h>
#define MAX_RECORDES 50

typedef struct recorde {
    unsigned long int pontos;
    char nome[4];
} RECORDE;

typedef struct lista_recordes {
    RECORDE recordes[MAX_RECORDES];
    int size;
} LISTA_RECORDES;

LISTA_RECORDES inicia_lista() {
    LISTA_RECORDES novo;
    novo.size = 0;
    return novo;
}

unsigned int size_recordes(LISTA_RECORDES lista) {
    return lista.size;
}

RECORDE* acessa_recorde(LISTA_RECORDES *lista, int idx) {
    return &lista->recordes[idx];
} 

void adiciona_recorde(LISTA_RECORDES* lista, unsigned long pontuacao, char letras[]) {
    if (size_recordes(*lista) >= MAX_RECORDES)
        return;
    acessa_recorde(lista, size_recordes(*lista))->pontos = pontuacao;
    strcpy(acessa_recorde(lista, size_recordes(*lista))->nome, letras);
    lista->size++;
}

void maiuscula(char string[]) {
    int i;
    for (i = 0; string[i] != '\0'; i++) {
        if (string[i] >= 'a' && string[i] <= 'z') {
            string[i] = (string[i] - 'a' + 'A');
        }
    }
}

void arquivar_recordes(LISTA_RECORDES lista) {
    int i;
    FILE *arquivo;
    if ((arquivo = fopen("recordes.dat", "w")) == NULL) {
        printf("Erro de arquivo");
        return;
    }
    fprintf(arquivo, "%d\n", lista.size);
    for (i = 0; i < lista.size; i++) {
        maiuscula(lista.recordes[i].nome);
        fprintf(arquivo, "%s %ld\n", lista.recordes[i].nome, lista.recordes[i].pontos);
    }
    fclose(arquivo);
}

void ler_recordes(LISTA_RECORDES *plista) {
    int i;
    FILE *arquivo;
    if ((arquivo = fopen("recordes.dat", "r")) == NULL) {
        printf("Erro de arquivo");
        return;
    }
    fscanf(arquivo, "%d", &plista->size);
    for (i = 0; i < size_recordes(*plista); i++) {
        fscanf(arquivo, "%s %ld", plista->recordes[i].nome, &plista->recordes[i].pontos);
    }
    fclose(arquivo);
}


int pontos(LISTA_RECORDES *lista, int idx) {
    return acessa_recorde(lista, idx)->pontos;
} 

void troca(RECORDE *a, RECORDE *b) {
    RECORDE temp;
    //temp = *a;
    strcpy(temp.nome, a->nome);
    temp.pontos = a->pontos;

    //*a = *b;
    strcpy(a->nome, b->nome);
    a->pontos = b->pontos;

    //*b = temp;
    strcpy(b->nome, temp.nome);
    b->pontos = temp.pontos;
}

void ordenar_recordes (LISTA_RECORDES *plista) {
    int i, j;
    for (i = 0; i < size_recordes(*plista); i++) {
        for (j = 0; j < (size_recordes(*plista) - 1); j++) {
            if (pontos(plista, j) < pontos(plista, j+1)) {
                troca(acessa_recorde(plista, j), acessa_recorde(plista, j+1));
            }
        }
    }
}

void transforma_recorde(int x, char resp[]) {
    int i = 0;
    int temp[10];
    for (i = 0; i < 10; i++) {
        temp[i] = 0;
    }
    i = 0;
    while (x > 0) {
        temp[i] = x % 10;
        x /= 10;
        i++;
    }
    for (i = 0; i < 9; i++) {
        resp[i] = (temp[9-i-1] + '0');
    }
    resp[10] = '\0';
}

void preencher_lista(LISTA_RECORDES* lista) {
    int i;
    for (i = MAX_RECORDES-1; i >= size_recordes(*lista); i--) {
        strcpy(lista->recordes[i].nome, "AAA");
        lista->recordes[i].pontos = 0;
    }
}
