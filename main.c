#include <stdio.h>
#include <stdlib.h>
#include "header/inimigo.h"
#include "header/recorde.h"
#include "header/projetil.h"
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <time.h>
#define FPS 60

SDL_Rect ajuste_camera(SDL_Rect referencial, SDL_Rect objeto);

SDL_Texture *carregar_img(char *, SDL_Renderer *);

char* itoa(int i, char b[]);

#define TELA_INICIO 1
#define TELA_MENU 2
#define TELA_JOGO 3
#define TELA_NOME 4
#define TELA_FIM 5

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600
#define oi(N) printf("oi %d\n", N)

#define ESQUERDA 1
#define DIREITA 0

int SCORE = 0;
int SCORE_BASE = 0;
int MULTIPLICADOR = 1;
unsigned VIDA = 3;

int main(void) {
    srand(time(NULL));
    //Inicia SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    IMG_Init(IMG_INIT_JPG);

    //Inicia SDL_ttf
    TTF_Init();

    // Iniciar o SDL_Mixer:
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 512);

    int gerar, nivel, qtd_sprites;

    //Cria uma Janela
    SDL_Window* janela = SDL_CreateWindow("Liberdade 2120",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,LARGURA_JANELA, ALTURA_JANELA, 0);

    //cria um render que usa o Hardware do computador
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* renderizador = SDL_CreateRenderer(janela, -1, render_flags);

    SDL_Surface* plano;
    SDL_Texture* palavra_score;
    SDL_Texture* pontos;
    SDL_Texture* textura_x;
    SDL_Texture* textura_multiplicador;

    //Carrega as texturas
    SDL_Texture* personagem = carregar_img("fonte/guy.png", renderizador);
    SDL_Texture* fundo = carregar_img("fonte/cyberpunk_.png", renderizador);
    SDL_Texture* estrelas = carregar_img("fonte/estrelas.png", renderizador);
    SDL_Texture* coracao = carregar_img("fonte/coracao.png", renderizador);
    SDL_Texture* personagem_caindo = carregar_img("fonte/caindo.png", renderizador);
    SDL_Texture* img_tiro = carregar_img("fonte/bullet.png", renderizador);
    SDL_Texture* voce_morreu = carregar_img("fonte/voce_morreu.png",renderizador);
    SDL_Texture* inimigos[3];
    SDL_Texture* inimigos_andando[3];

    inimigos_andando[0] = carregar_img("fonte/ninja1/correndo.png", renderizador);
    inimigos_andando[1] = carregar_img("fonte/ninja2/correndo.png", renderizador);
    inimigos_andando[2] = carregar_img("fonte/ninja3/correndo.png", renderizador);

    SDL_Rect sprite_ninjas[3];
    setrect(&sprite_ninjas[0], 0, 0, 36, 26);
    setrect(&sprite_ninjas[1], 0, 0, 36, 26);
    setrect(&sprite_ninjas[2], 0, 0, 32, 32);

    SDL_Texture* inimigos_atacando[3];

    inimigos_atacando[0] = carregar_img("fonte/ninja1/atacando.png", renderizador);
    inimigos_atacando[1] = carregar_img("fonte/ninja2/atacando.png", renderizador);
    inimigos_atacando[2] = carregar_img("fonte/ninja3/atacando.png", renderizador);

    SDL_Rect sprite_ninjas_atacando[3];
    setrect(&sprite_ninjas_atacando[0], 0, 0, 42, 26);
    setrect(&sprite_ninjas_atacando[1], 0, 0, 42, 26);
    setrect(&sprite_ninjas_atacando[2], 0, 0, 32, 32);

    SDL_Rect sp_tiro = {0, 0, 500, 500};
    SDL_Rect ponto_tiro;

    SDL_Rect pos_personagem = {LARGURA_JANELA/2 - 60, ALTURA_PERSONAGEM, 160,160};
    SDL_Rect camera = {0, 0, LARGURA_JANELA, ALTURA_JANELA};
    SDL_Rect cam_estrelas = {0 , 0 , LARGURA_JANELA, ALTURA_JANELA};

    SDL_Rect auxiliar ;
    SDL_Rect sprite_personagem = { 0, 0, 45, 45};

    SDL_Rect heart[3];
    setrect(&heart[0], 640, 30, 30, 30);
    setrect(&heart[1], 670, 30, 30, 30);
    setrect(&heart[2], 700, 30, 30, 30);

    //propriedades da fonte
    SDL_Color White = {255, 255, 255}; 
    SDL_Rect rect_mensagem = {30, 30, 100, 25};
    SDL_Rect rect_x = {360, 30, 30, 40};

    //Carregar fonte
    TTF_Font* fonte = TTF_OpenFont("fonte/fonte.ttf", 24);
    plano = TTF_RenderText_Solid(fonte, "Score: ", White); 
    palavra_score = SDL_CreateTextureFromSurface(renderizador, plano);
    SDL_FreeSurface(plano);

    plano = TTF_RenderText_Solid(fonte, "X", White); 
    textura_x = SDL_CreateTextureFromSurface(renderizador, plano);
    SDL_FreeSurface(plano);

    if (fonte == NULL) {
        printf("Erro ao carregar fonte: %s\n", TTF_GetError());
    }

    //Propriedades SOM
    Mix_Chunk* tiro = Mix_LoadWAV("fonte/tiro.wav");
    Mix_Chunk* dor = Mix_LoadWAV("fonte/dor.wav");
    Mix_Chunk* espada1 = Mix_LoadWAV("fonte/espada1.wav");
    Mix_Chunk* morte = Mix_LoadWAV("fonte/morte.wav");

    Mix_Music* yo = Mix_LoadMUS("fonte/yo.mp3");
    Mix_Music* musica_menu = Mix_LoadMUS("fonte/tela_menu.mp3");
    Mix_Music* musica_jogo = Mix_LoadMUS("fonte/tela_jogo.mp3");

    //loop de jogo;
    int rodando = TELA_INICIO;
    SDL_Event evento;
    int direita = 0, esquerda = 0, atirando = 0;
    int ultima_direcao = DIREITA;
    SDL_RendererFlip rotacao_personagem;
    const Uint8* keystate;
    int i, j;

    int inimigo_batendo = 0, vel_inimigo = 6;

    //Spawn inimigos
    int pos_i, face;
    float mult_sprite;

    //Contadores
    unsigned int count = 0, count_secundario = 0, star_count = 0, count_morte = 0, count_porrada = 0, count_tiros = 0, cont_pointer = 0;
    unsigned int count_musica = 0;
    unsigned int tempo_atual, ultimo_tempo;

    //Gerando coleÃ§Ã£o de inimigos e tiros
    COLECAO_INIMIGOS *colecao;
    CONJUNTO_PROJETEIS *conjunto_tiros;

    //Armazenar recordes
    LISTA_RECORDES lista_rec;
    lista_rec = inicia_lista();

    char score[6];
    char multiplicador[3];
    char nome_jogador[4];
    int count_nome = 0;
    int q = 1, p = 1, count_multiplicador;
    
    while (rodando) {
        ultimo_tempo = SDL_GetTicks();

        if (rodando == TELA_INICIO) {
            //Tela Inicio

            SDL_Rect rect_inicio = {0, 0, 800, 600};
            SDL_Texture* inicio = carregar_img("fonte/inicio_jogo.jpg", renderizador);
            SDL_RenderClear(renderizador);
            SDL_RenderCopy(renderizador, inicio, NULL, &rect_inicio);
            SDL_RenderPresent(renderizador);
            SDL_Delay(30000);
            SDL_DestroyTexture(inicio);
            rodando = TELA_MENU;
        }

        if (rodando == TELA_MENU) {

            if (count_musica == 0) {
                Mix_PlayMusic(musica_menu, -1);
                count_musica = 1;
            }

            SCORE = 0;
            SCORE_BASE = 0;
            MULTIPLICADOR = 1;
            VIDA = 3;
            p = 1;
            q = 1;
            vel_inimigo = 6;
            count_multiplicador = 0;
            camera.x = pos_personagem.x - LARGURA_JANELA/2 + pos_personagem.w/2;
            pos_personagem.x = LARGURA_JANELA/2 - pos_personagem.w/2;
            ultima_direcao = direita;

            ler_recordes(&lista_rec);
            ordenar_recordes(&lista_rec);
            preencher_lista(&lista_rec);

            SDL_Rect rect_menu = {0, 0, 800, 600};
            SDL_Texture* menu = carregar_img("fonte/liberdade_2120.png", renderizador);
            SDL_RenderClear(renderizador);
            SDL_RenderCopy(renderizador, menu, NULL, &rect_menu);
            SDL_DestroyTexture(menu);

            while (SDL_PollEvent(&evento)) {
                if (evento.type == SDL_QUIT) {
                    if (cont_pointer > 0) {
                        free(colecao);
                        free(conjunto_tiros);
                        cont_pointer = 0;
                    }
                    rodando = 0;
                }
                else if (evento.type == SDL_KEYDOWN) {
                    if (evento.key.keysym.sym == SDLK_ESCAPE) {
                        rodando = 0;
                    }
                    else if (evento.key.keysym.sym == SDLK_SPACE) {
                        colecao = criar_lista_inimigos(50);
                        conjunto_tiros = gera_projeteis();
                        cont_pointer++;
                        count_musica = 0;
                        rodando = TELA_JOGO;
                        count_morte = 0;
                        count_porrada = 0;
                        count_tiros = 20;
                        Mix_HaltMusic();
                        Mix_PlayMusic(musica_jogo, -1);
                    }
                    else if (evento.key.keysym.sym == SDLK_r) {
                        rodando = TELA_FIM;
                    }
                }
            }
        }
        else if(rodando == TELA_JOGO) {
            gerar = 0;
            
            SCORE = SCORE_BASE;

            SDL_RenderClear(renderizador);
            if (SCORE == 0) {
                p = 1;
            }
            else { 
                q = log10(SCORE) + 1;
            }   
            p = log10(MULTIPLICADOR) + 1;

            if (direita && !esquerda) {
                count_multiplicador++;
                if (count_multiplicador == 60) {
                    MULTIPLICADOR++;
                    count_multiplicador = 0;
                }
            }

            SDL_Rect rect_pontos = {130, 30, 20*q, 25};
            plano = TTF_RenderText_Solid(fonte, itoa(SCORE, score), White); 
            pontos = SDL_CreateTextureFromSurface(renderizador, plano);
            SDL_FreeSurface(plano);

            SDL_Rect rect_multiplicador = {400, 30, 25*p, 40};
            plano = TTF_RenderText_Solid(fonte, itoa(MULTIPLICADOR, multiplicador), White); 
            textura_multiplicador = SDL_CreateTextureFromSurface(renderizador, plano);
            SDL_FreeSurface(plano);

            

            while (SDL_PollEvent(&evento))
            {
                if (evento.type == SDL_QUIT) {
                    if (cont_pointer > 0) {
                        free(colecao);
                        free(conjunto_tiros);
                        cont_pointer = 0;
                    }
                    rodando = 0;
                }
                else if (evento.type == SDL_KEYDOWN) {
                    if (evento.key.keysym.sym == SDLK_ESCAPE) {
                        rodando = 0;
                    }
                    else if (evento.key.keysym.sym == SDLK_RIGHT) {
                        direita = 1;
                        ultima_direcao = DIREITA;
                    }

                    else if (evento.key.keysym.sym == SDLK_LEFT) {
                        esquerda = 1;
                        ultima_direcao = ESQUERDA;
                    }

                    else if (evento.key.keysym.sym == SDLK_z) {
                        ultima_direcao = ESQUERDA;
                        atirando = 1;
                        
                        if (count_tiros == 0 || count_tiros % 20 == 0) {
                            Mix_PlayChannel(-1, tiro, 0);
                            setrect(&ponto_tiro, 380 - (pos_personagem.w/2), ALTURA_PERSONAGEM+65, 50, 50);
                            surge_projetil(conjunto_tiros, ponto_tiro, sp_tiro, ultima_direcao);
                            count_tiros = 0;
                        }
                        count_tiros++;
                    }
                    else if (evento.key.keysym.sym == SDLK_x) {
                        ultima_direcao = DIREITA;
                        atirando = 1;

                        if (count_tiros == 0|| count_tiros % 20 == 0) {
                            Mix_PlayChannel(-1, tiro, 0);
                            setrect(&ponto_tiro, 380 + (pos_personagem.w/2), ALTURA_PERSONAGEM+65, 50, 50);
                            surge_projetil(conjunto_tiros, ponto_tiro, sp_tiro, ultima_direcao);
                            count_tiros = 0;
                        }
                        count_tiros++;
                    }
                    

                }
                else if (evento.type == SDL_KEYUP) {
                    if (evento.key.keysym.sym == SDLK_RIGHT) {
                        direita = 0;
                    }
                    else if (evento.key.keysym.sym == SDLK_LEFT) {
                        esquerda = 0;
                    }
                    else if (evento.key.keysym.sym == SDLK_z) {
                        if (direita){
                            ultima_direcao = DIREITA;
                        }
                        atirando = 0;

                        keystate = SDL_GetKeyboardState(NULL);
                        if (keystate[SDLK_x]) {
                            atirando = 1;
                            ultima_direcao = DIREITA;
                        }
                        count_tiros = 0;
                    }
                    else if (evento.key.keysym.sym == SDLK_x) {
                        if (esquerda) {
                            ultima_direcao = ESQUERDA;
                        }
                        atirando = 0;

                        keystate = SDL_GetKeyboardState(NULL);
                        if (keystate[SDLK_z]) {
                            atirando = 1;
                            ultima_direcao = ESQUERDA;
                        }
                        count_tiros = 0;
                    }
                }

            }
            if (direita) {
                pos_personagem.x += 8;
            }
            if (esquerda) {
                pos_personagem.x -= 8;
            }

            if (camera.x  + LARGURA_JANELA > 1900) {
                pos_personagem.x = LARGURA_JANELA/2 - pos_personagem.w/2;
            } 

            if (camera.x < 0) {
                pos_personagem.x = 1100 + LARGURA_JANELA/2 - pos_personagem.w/2 - 1;
            }

            camera.x = pos_personagem.x - LARGURA_JANELA/2 + pos_personagem.w/2;

            if (cam_estrelas.x + LARGURA_JANELA> 1900) {
                cam_estrelas.x = 0;
            }
            if (star_count % 5 == 0) {
                cam_estrelas.x++;
                star_count = 0;
            }
            star_count++;

            //limpa a janela
            SDL_RenderClear(renderizador);

            //Desenha fundo na tela

            SDL_RenderCopy(renderizador, estrelas, &cam_estrelas, NULL);

            SDL_RenderCopy(renderizador, fundo, &camera, NULL);

            if (count == 6) {
                count_secundario++;
                if ((VIDA > 0) && ((count_secundario % 5 == 2))) {
                    gerar = 1;
                }
                count = 0;
            }
            
            if (gerar > 0) {
                if (SCORE >= 100000) {
                    vel_inimigo = 11;
                    nivel = rand()%3;
                }
                else if (SCORE >= 50000) {
                    vel_inimigo = 9;
                    nivel = rand()%3;
                }
                else if (SCORE >= 10000) {
                    vel_inimigo = 8;
                    nivel = rand()%3;
                }
                else if (SCORE >= 1000) {
                    nivel = rand()%2;
                    vel_inimigo = 7;
                }
                else {
                    vel_inimigo = 6;
                    nivel = 0;
                }

                if (nivel == 2) {
                    mult_sprite = 3.5;
                }
                else
                    mult_sprite = 4;

                face = rand()%2;
                pos_i = MIN_POS(face);
                insere_inimigo(colecao, pos_i, nivel, face, (sprite_ninjas[nivel].w * mult_sprite), (sprite_ninjas[nivel].h * mult_sprite));
                setrect_direto(sprite_inimigo(colecao, ultimo_adicionado(colecao)), sprite_ninjas[nivel]);
                gerar = 0;
            }

            if (VIDA == 0) {
                if (count_musica == 0) {
                    Mix_PlayChannel(-1, morte, 0);
                    count_musica = 1;
                }
                if (count_morte / 8 >= 5) {
                    if (cont_pointer > 0) {
                        free(colecao);
                        free(conjunto_tiros);
                        cont_pointer = 0;
                    }
                    SDL_Delay(1500);
                    count_nome = 0;
                    count_secundario = 0;
                    Mix_HaltMusic();
                    count_musica = 0;
                    Mix_PlayMusic(yo, 0);
                    rodando = TELA_NOME;
                    continue;
                }
                sprite_personagem.y = 0;
                sprite_personagem.x = sprite_personagem.w*((count_morte/8)%5);
                count_morte++;
            }
            
            else if ((!direita && !esquerda) || (direita && esquerda)) {

                if (atirando == 0) {
                    sprite_personagem.y = 0;
                    sprite_personagem.x = sprite_personagem.w*(count_secundario%6);
                }
                else {
                    sprite_personagem. y = sprite_personagem.h*2;
                    sprite_personagem.x = sprite_personagem.w*(count_secundario%2) + 2*sprite_personagem.w;
                }
            }

            else {
                if (atirando == 1) {
                    sprite_personagem.y = 3*sprite_personagem.h;
                    sprite_personagem.x = sprite_personagem.w*(count_secundario % 8);
                }
                else {
                    if (count_secundario % 8 < 2) {
                        sprite_personagem.y = 0;
                        sprite_personagem.x = sprite_personagem.w*((count_secundario % 2)) + 6*sprite_personagem.w;
                    }
                    else {
                        sprite_personagem.y = sprite_personagem.h*1;
                        sprite_personagem.x = sprite_personagem.w*((count_secundario % 8) - 2);
                    }
                }
            }

            //Desenha a personagem na tela
            if (ultima_direcao) {
                rotacao_personagem = SDL_FLIP_HORIZONTAL;
            }
            else {
                rotacao_personagem = SDL_FLIP_NONE;
            }
            auxiliar = ajuste_camera(camera,pos_personagem);
            if (VIDA == 0) {
                SDL_RenderCopyEx(renderizador, personagem_caindo, &sprite_personagem, &auxiliar,0 ,NULL, rotacao_personagem);
            }
            else {
                SDL_RenderCopyEx(renderizador, personagem, &sprite_personagem, &auxiliar,0 ,NULL, rotacao_personagem);
            }

            //Faz projÃ©teis andarem na tela
            
            for (i = 0; i < quant_tiros(conjunto_tiros); i++) {
                
                move_tiro(conjunto_tiros, i);
                SDL_RenderCopyEx(renderizador, img_tiro, sprite_tiro(conjunto_tiros, i), pos_tiro(conjunto_tiros, i),0 ,NULL, face_tiro(conjunto_tiros, i));

                if (pos_tiro(conjunto_tiros, i)->x >= MIN_POS(1) ||pos_tiro(conjunto_tiros, i)->x <= MIN_POS(0)) {
                    if (acessa_projetil(conjunto_tiros, i) != NULL)
                        remove_tiro(conjunto_tiros, i);
                    continue;
                }

                for (j = 0; j < size_inimigos(colecao); j++) {
                    if (size_inimigos(colecao) <= 0) {
                        break;
                    }
                    if (face_inimigo(colecao, j) == 0) {
                        if (face_tiro(conjunto_tiros, i) == 1) {
                            if (distancia(pos_tiro(conjunto_tiros, i)->x, (pos_inimigo(colecao, j)->x)) <= pos_inimigo(colecao, j)->w/2) {
                                if (*(vida_inimigo(colecao, j)) > 0) {
                                    *(vida_inimigo(colecao, j)) -= 1;
                                }
                                if (acessa_projetil(conjunto_tiros, i) != NULL) {
                                    remove_tiro(conjunto_tiros, i);
                                }
                                break;
                            }
                        }

                    }
                    else {
                        if (face_tiro(conjunto_tiros, i) == 0) {
                            if (distancia((pos_inimigo(colecao, j)->x), pos_tiro(conjunto_tiros, i)->x) <= pos_inimigo(colecao, j)->w/2) {
                                if (*(vida_inimigo(colecao, j)) > 0) {
                                    *(vida_inimigo(colecao, j)) -= 1;
                                }
                                if (acessa_projetil(conjunto_tiros, i) != NULL) {
                                    remove_tiro(conjunto_tiros, i);
                                }
                                break;
                            }
                        }
                    }
                }
            }

            //Remover os tiros fora
            for (i = 0; i < size_inimigos(colecao); i++) {
                if (size_inimigos(colecao) <= 0) {
                    break;
                }
                if (*(vida_inimigo(colecao, i)) == 0) {
                    SCORE_BASE += (10 * MULTIPLICADOR * (nivel_inimigo(colecao, i)+1));
                    remove_inimigo(colecao, i);
                    i--;
                    continue;
                }
            }
            
            //Inimigos dando dano
            for (i = 0; i < size_inimigos(colecao); i++) {
                if (size_inimigos(colecao) <= 0) {
                        break;
                }
                
                if (*bateu(colecao, i) > 0) {
                    count_porrada++;
                    break;
                }
                if (((count_porrada/6) > 0) && VIDA > 0) {
                    Mix_PlayChannel(-1, espada1, 0);
                    Mix_PlayChannel(-1, dor, 0);
                    VIDA--;
                    count_porrada = 0;
                    if (MULTIPLICADOR > 1) {
                        MULTIPLICADOR = MULTIPLICADOR/2;
                    }
                    break;
                }
            }

            //Desenha inimigo na tela

            for (i = 0; i < size_inimigos(colecao); i++) {
                inimigo_batendo = andar_inimigo(colecao, i, vel_inimigo);
                if (!inimigo_batendo) {
                    switch (nivel_inimigo(colecao, i)) {
                        case 0:
                        case 1:
                            qtd_sprites = 4;
                            break;
                        case 2:
                            qtd_sprites = 6;
                            break;
                        default:
                            break;
                    }
                    muda_imagem(colecao, i, sprite_ninjas_atacando[nivel_inimigo(colecao, i)], qtd_sprites);
                    inimigos[nivel_inimigo(colecao, i)] = inimigos_atacando[nivel_inimigo(colecao, i)];
                    muda_sprite(colecao, i, count_secundario);
                    
                    if (count_secundario % qtd_sprites >= qtd_sprites - 1) {
                        *bateu(colecao, i) += 1;
                    }
                    else {
                        *bateu(colecao, i) = 0;
                    }
                    
                }
                else {
                    inimigos[nivel_inimigo(colecao, i)] = inimigos_andando[nivel_inimigo(colecao, i)];
                    muda_sprite(colecao, i, count_secundario);
                }
                
                SDL_RenderCopyEx(renderizador, inimigos[nivel_inimigo(colecao, i)], sprite_inimigo(colecao, i), pos_inimigo(colecao, i), 0, NULL, face_inimigo(colecao, i));
            }
            
            //Mostra o score
            SDL_RenderCopy(renderizador, palavra_score, NULL, &rect_mensagem);
            SDL_RenderCopy(renderizador, pontos, NULL, &rect_pontos);

            //Mostra multiplicador
            SDL_RenderCopy(renderizador, textura_x, NULL, &rect_x);
            SDL_RenderCopy(renderizador, textura_multiplicador, NULL, &rect_multiplicador);

            //Desenha coraÃ§Ã£o na tela
            for (i = 0; i < VIDA; i++) {
                SDL_RenderCopy(renderizador, coracao, NULL, &heart[i]);
            }
            
            SDL_DestroyTexture(pontos);
            SDL_DestroyTexture(textura_multiplicador);

            count++;
        }
        else if (rodando == TELA_NOME) {

            SDL_RenderClear(renderizador);

            SDL_RenderCopy(renderizador, voce_morreu, NULL, NULL);
            plano = TTF_RenderText_Solid(fonte, "Digite seu nome:", White); 
            SDL_Texture* pede_nome = SDL_CreateTextureFromSurface(renderizador, plano);
            SDL_FreeSurface(plano);
            SDL_Rect rect_pede_nome = {225, 200, 350, 50};
            SDL_RenderCopy(renderizador, pede_nome, NULL, &rect_pede_nome);
            SDL_DestroyTexture(pede_nome);

            plano = TTF_RenderText_Solid(fonte, "_", White);
            SDL_Texture* underline = SDL_CreateTextureFromSurface(renderizador, plano);
            SDL_FreeSurface(plano);

            SDL_Rect underlines[3];
            SDL_Texture* letras[3];
            SDL_Rect pos_letras[3];
            char temp_string[2];

            for (i = 0; i < count_nome; i++) {
                if (i >= 3)
                    break;

                temp_string[0] = nome_jogador[i];
                temp_string[1] = '\0';

                plano = TTF_RenderText_Solid(fonte, temp_string, White);
                letras[i] = SDL_CreateTextureFromSurface(renderizador, plano);
                SDL_FreeSurface(plano);

                setrect(&pos_letras[i], 215 + (150*i), 325, 50, 75);
                SDL_RenderCopy(renderizador, letras[i], NULL, &(pos_letras[i]));
                SDL_DestroyTexture(letras[i]);
            }
            
            for (i = 0; i < 3; i++) {
                setrect(&underlines[i], 200 + (150*i) ,400, 100, 25);
                SDL_RenderCopy(renderizador, underline, NULL, &(underlines[i]));
            }
            SDL_DestroyTexture(underline);

            SDL_RenderPresent(renderizador);
            while (SDL_PollEvent(&evento))
            {
                if (evento.type == SDL_QUIT) {
                    if (cont_pointer > 0) {
                        free(colecao);
                        free(conjunto_tiros);
                        cont_pointer = 0;
                    }
                    rodando = 0;
                }
                else if (evento.type == SDL_KEYDOWN) {
                    if (evento.key.keysym.sym == SDLK_ESCAPE) {
                        rodando = 0;
                    }
                    else if (evento.key.keysym.sym >= 'a' && evento.key.keysym.sym <= 'z') {
                        if (count_nome < 3) {
                            nome_jogador[count_nome] = evento.key.keysym.sym;
                            count_nome++;
                        }
                        
                    }
                    else if (evento.key.keysym.sym == SDLK_SPACE) {
                        break;
                    }
                    else if (evento.key.keysym.sym == SDLK_BACKSPACE) {
                        if (count_nome > 0) {
                            nome_jogador[count_nome - 1] = '\0';
                            count_nome--;
                        }
                    }
                }
            }
            if (count_nome > 3) {
                SDL_Delay(1000);
                nome_jogador[3] = '\0';

                adiciona_recorde(&lista_rec, SCORE, nome_jogador);
                ordenar_recordes(&lista_rec);
                arquivar_recordes(lista_rec);

                rodando = TELA_FIM;
            }
            else if (count_nome == 3) {
                count_nome++;
                continue;
            }
        }
        
        else if(rodando == TELA_FIM) {
            char temp[11];
            
            ler_recordes(&lista_rec);
            ordenar_recordes(&lista_rec);
            preencher_lista(&lista_rec);


            SDL_Rect pontuacao = {330,190,300,30};
            SDL_Rect nome_recorde = {180,190,100,30};
            SDL_Rect rect_fim = {0, 0, 800, 600};

            SDL_Texture* fim = carregar_img("fonte/tela_final.png", renderizador);
            
            SDL_RenderClear(renderizador);
            SDL_RenderCopy(renderizador, fim, NULL, &rect_fim);

            //Imprimir maiores pontuaÃ§Ãµes
            for (i = 0; i < 5; i++) {
                
                plano = TTF_RenderText_Solid(fonte, lista_rec.recordes[i].nome, White);
                SDL_Texture* nome_rec = SDL_CreateTextureFromSurface(renderizador, plano);
                SDL_FreeSurface(plano);
                transforma_recorde(lista_rec.recordes[i].pontos, temp);

                plano = TTF_RenderText_Solid(fonte, temp, White);
                SDL_Texture* pontos_rec = SDL_CreateTextureFromSurface(renderizador, plano);
                SDL_FreeSurface(plano);

                SDL_RenderCopy(renderizador, nome_rec, NULL, &nome_recorde);
                SDL_RenderCopy(renderizador, pontos_rec, NULL, &pontuacao);

                pontuacao.y += 60;
                nome_recorde.y += 60;

                SDL_DestroyTexture(nome_rec);
                SDL_DestroyTexture(pontos_rec);
            }
            
            SDL_RenderPresent(renderizador);
            SDL_DestroyTexture(fim);
            
            while (SDL_PollEvent(&evento)) {
                if (evento.type == SDL_QUIT) {
                    if (cont_pointer > 0) {
                        free(colecao);
                        free(conjunto_tiros);
                        cont_pointer = 0;
                    }
                    rodando = 0;
                }
                else if (evento.type == SDL_KEYDOWN) {
                    if (evento.key.keysym.sym == SDLK_ESCAPE) {
                        Mix_HaltMusic();
                        rodando = 0;
                    }
                    else if (evento.key.keysym.sym == SDLK_SPACE){
                        count_musica = 0;
                        Mix_HaltMusic();
                        rodando = TELA_MENU;
                    }
                }
            }
        }

        SDL_RenderPresent(renderizador);

        tempo_atual = SDL_GetTicks();
        
        //Espera 1/60 de segundo (para o prÃ³ximo frame)
        SDL_Delay(1000/FPS - (SDL_TICKS_PASSED(ultimo_tempo,tempo_atual)));

    }
    //FIM DO LOOP
    if (cont_pointer > 0) {
        free(colecao);
        free(conjunto_tiros);
        cont_pointer = 0;
    }
    //Limpa a janela
    SDL_RenderClear(renderizador);
    
    //Limpa oq temos antes de fechar
    SDL_DestroyTexture(personagem);
    SDL_DestroyTexture(textura_x);
    SDL_DestroyTexture(textura_multiplicador);
    SDL_DestroyTexture(palavra_score);
    SDL_DestroyRenderer(renderizador);
    SDL_RenderClear(renderizador);
    SDL_DestroyWindow(janela);
    
    Mix_Quit();
    SDL_Quit();

    return 0;
}

SDL_Rect ajuste_camera(SDL_Rect referencial, SDL_Rect objeto) {

    SDL_Rect saida = { objeto.x - referencial.x , objeto.y - referencial.y , objeto.w, objeto.h};

    return saida;
}

SDL_Texture *carregar_img(char *img, SDL_Renderer *renderer) {
    SDL_Texture *a;
    SDL_Surface* foto = IMG_Load(img);
    a = SDL_CreateTextureFromSurface(renderer, foto);
    SDL_FreeSurface(foto);
    return a;
}

char* itoa(int i, char b[]) {
    char const digit[] = "0123456789";
    char *p = b;
    if(i < 0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do { 
        ++p;
        shifter = shifter/10;
    } while(shifter);
    *p = '\0';
    do { 
        *--p = digit[i%10];
        i = i/10;
    } while(i);
    return b;
}