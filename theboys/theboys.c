#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "conjunto.h"
#include "fila.h"

#define FIM_DO_MUNDO 5256
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

#define ALEAT(a, b) ((rand() % (b)) + (a))

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

struct Mundo {
    struct Base bases[N_BASES];
    struct Heroi herois[N_HEROIS];
    struct Missao missoes[N_MISSOES];
    struct Coordenada tamanho;
};

void imprime_missao(struct Missao m, int id)
{
    printf("Missao %d\n", id);
    printf("  Habilidades: ");
    printf("%d\n", m.habilidades->card);
    imprime_cjt(m.habilidades);
    printf("\n");
    printf("  Localizacao: (%d, %d)\n", m.local.x, m.local.y);
}

void inicializa_missoes(struct Mundo *mundo)
{
    int i, j;
    for (i = 0; i < N_MISSOES; i++) {
        mundo->missoes[i].local.x = ALEAT(0, mundo->tamanho.x);
        mundo->missoes[i].local.y = ALEAT(0, mundo->tamanho.y);
        mundo->missoes[i].habilidades = cria_cjt(N_HABILIDADES);
        for (j = 0; j < N_HABILIDADES; j++)
            insere_cjt(mundo->missoes[i].habilidades, ALEAT(0, N_HABILIDADES));
    }
}

void destroi_missoes(struct Mundo *mundo)
{
    int i;
    for (i = 0; i < N_MISSOES; i++)
        destroi_cjt(mundo->missoes[i].habilidades);
}

int main()
{
    int i;
    struct Mundo mundo;
    mundo.tamanho.x = 20000;
    mundo.tamanho.y = 20000;

    srand(0); /* use zero, nao faca com time (0) */

    inicializa_missoes(&mundo);
    for (i = 0; i < N_MISSOES; i++)
        imprime_missao(mundo.missoes[i], i);

    destroi_missoes(&mundo);

    return 0;
}
