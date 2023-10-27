#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "conjunto.h"
#include "fila.h"

/* a configuracao da simulacao so eh realmente usada na main, todas as outras
 * funcoes recebem configuracoes como argumento. de qualquer maneira, eh mais
 * simples vir aqui em cima e alterar */
#define FIM_DO_MUNDO 525600
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 6)
#define N_MISSOES (FIM_DO_MUNDO / 100)
#define MIN_PACIENCIA 0
#define MAX_PACIENCIA 100
#define MIN_VELOCIDADE 50
#define MAX_VELOCIDADE 5000
#define MIN_LOTACAO 3
#define MAX_LOTACAO 10

/* usados como valores de inicializacao e etc */
#define BASE_INVALIDA (-1)
#define HEROI_INVALIDO (-1)
#define HABILIDADE_INVALIDA (-1)

#define ALEAT(a, b) ((rand() % ((b) + 1)) + (a))

struct Coordenada {
    int x;
    int y;
};

struct Base {
    int lotacao;
    struct conjunto *herois;
    struct fila *espera;
    struct Coordenada local;
};

struct Heroi {
    struct conjunto *habilidades;
    int paciencia;
    int velocidade;
    int experiencia;
    int base_id;
};

struct Missao {
    struct conjunto *habilidades;
    struct Coordenada local;
};

void inicializa_herois(struct Heroi herois[],
                       int n,
                       int n_hab,
                       int min_paciencia,
                       int max_paciencia,
                       int min_velocidade,
                       int max_velocidade)
{
    int i, j, max_hab_heroi;
    for (i = 0; i < n; i++) {
        herois[i].base_id = BASE_INVALIDA;
        herois[i].experiencia = 0;
        herois[i].paciencia = ALEAT(min_paciencia, max_paciencia);
        herois[i].velocidade = ALEAT(min_velocidade, max_velocidade);
        max_hab_heroi = ALEAT(1, n_hab);
        herois[i].habilidades = cria_cjt(max_hab_heroi);
        for (j = 0; j < max_hab_heroi; j++)
            insere_cjt(herois[i].habilidades, ALEAT(0, n_hab));
    }
}

void destroi_herois(struct Heroi herois[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        destroi_cjt(herois[i].habilidades);
}

void inicializa_bases(struct Base bases[],
                      int n,
                      int maxx,
                      int maxy,
                      int min_lotacao,
                      int max_lotacao)
{
    int i;
    for (i = 0; i < n; i++) {
        bases[i].local = (struct Coordenada) {
            ALEAT(0, maxx), ALEAT(0, maxy)
        };
        bases[i].lotacao = ALEAT(min_lotacao, max_lotacao);
        bases[i].herois = cria_cjt(bases[i].lotacao);
        bases[i].espera = fila_cria();
    }
}

void destroi_bases(struct Base bases[], int n)
{
    int i;
    for (i = 0; i < n; i++) {
        destroi_cjt(bases[i].herois);
        fila_destroi(&bases[i].espera);
    }
}

void inicializa_missoes(struct Missao missoes[],
                        int n,
                        int n_hab,
                        int maxx,
                        int maxy)
{
    int i, j, max_hab_missao;
    for (i = 0; i < n; i++) {
        missoes[i].local.x = ALEAT(0, maxx);
        missoes[i].local.y = ALEAT(0, maxy);
        max_hab_missao = ALEAT(1, n_hab);
        missoes[i].habilidades = cria_cjt(max_hab_missao);
        for (j = 0; j < max_hab_missao; j++)
            insere_cjt(missoes[i].habilidades, ALEAT(0, n_hab));
    }
}

void destroi_missoes(struct Missao missoes[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        destroi_cjt(missoes[i].habilidades);
}

struct Mundo {
    struct Base bases[N_BASES];
    struct Heroi herois[N_HEROIS];
    struct Missao missoes[N_MISSOES];
    struct Coordenada tamanho;
};

static struct Mundo mundo;

int main()
{
    mundo.tamanho.x = 20000;
    mundo.tamanho.y = 20000;

    srand(0);

    inicializa_missoes(mundo.missoes,
                       N_MISSOES,
                       N_HABILIDADES,
                       mundo.tamanho.x,
                       mundo.tamanho.y);
    inicializa_herois(mundo.herois,
                      N_HEROIS,
                      N_HABILIDADES,
                      MIN_PACIENCIA,
                      MAX_PACIENCIA,
                      MIN_VELOCIDADE,
                      MAX_VELOCIDADE);
    inicializa_bases(mundo.bases,
                     N_BASES,
                     mundo.tamanho.x,
                     mundo.tamanho.y,
                     MIN_LOTACAO,
                     MAX_LOTACAO);

    destroi_missoes(mundo.missoes, N_MISSOES);
    destroi_herois(mundo.herois, N_HEROIS);
    destroi_bases(mundo.bases, N_BASES);

    return 0;
}
