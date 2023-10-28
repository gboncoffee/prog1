#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "conjunto.h"
#include "fila.h"
#include "lef.h"

/* todas as funcoes que retornam int para indicar falha retornam 1 em sucesso e
 * 0 em falha */

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
#define MAX_TEMPO_CHEGADA 4320

/* usados como valores de inicializacao e etc */
#define BASE_INVALIDA (-1)
#define HEROI_INVALIDO (-1)
#define HABILIDADE_INVALIDA (-1)

/* para manter os eventos como um int eu nao uso um enum */
#define EV_INVALIDO (-1)
#define EV_CHEGA 0
#define EV_ESPERA 1
#define EV_DESISTE 2
#define EV_AVISA 3
#define EV_ENTRA 4
#define EV_SAI 5
#define EV_VIAJA 6
#define EV_MISSAO 7
#define EV_FIM_DO_MUNDO 8

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

struct Mundo {
    struct Base bases[N_BASES];
    struct Heroi herois[N_HEROIS];
    struct Missao missoes[N_MISSOES];
    struct lef_t *eventos;
    struct Coordenada tamanho;
};

int inicializa_herois(struct Heroi herois[])
{
    int i, j, max_hab_heroi;
    for (i = 0; i < N_HEROIS; i++) {
        herois[i].base_id = BASE_INVALIDA;
        herois[i].experiencia = 0;
        herois[i].paciencia = ALEAT(MIN_PACIENCIA, MAX_PACIENCIA);
        herois[i].velocidade = ALEAT(MIN_VELOCIDADE, MAX_VELOCIDADE);
        max_hab_heroi = ALEAT(1, N_HABILIDADES);
        if (!(herois[i].habilidades = cria_cjt(max_hab_heroi)))
            return 0;
        for (j = 0; j < max_hab_heroi; j++)
            insere_cjt(herois[i].habilidades, ALEAT(0, N_HABILIDADES));
    }

    return 1;
}

/* mais interessante passar o numero de herois para o caso de querer usar essa
 * funcao para destruir so algumas estruturas e etc */
void destroi_herois(struct Heroi herois[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        destroi_cjt(herois[i].habilidades);
}

int inicializa_bases(struct Base bases[], int maxx, int maxy)
{
    int i;
    for (i = 0; i < N_BASES; i++) {
        bases[i].local = (struct Coordenada) {
            ALEAT(0, maxx), ALEAT(0, maxy)
        };
        bases[i].lotacao = ALEAT(MIN_LOTACAO, MAX_LOTACAO);
        bases[i].herois = cria_cjt(bases[i].lotacao);
        bases[i].espera = fila_cria();
        if (!(bases[i].herois && bases[i].espera))
            return 0;
    }

    return 1;
}

/* passa o numero pelo mesmo motivo de destroi_herois */
void destroi_bases(struct Base bases[], int n)
{
    int i;
    for (i = 0; i < n; i++) {
        if (bases[i].herois != NULL)
            destroi_cjt(bases[i].herois);
        if (bases[i].espera != NULL)
            fila_destroi(&bases[i].espera);
    }
}

int inicializa_missoes(struct Missao missoes[], int maxx, int maxy)
{
    int i, j, max_hab_missao;
    for (i = 0; i < N_MISSOES; i++) {
        missoes[i].local.x = ALEAT(0, maxx);
        missoes[i].local.y = ALEAT(0, maxy);
        max_hab_missao = ALEAT(1, N_HABILIDADES);
        if (!(missoes[i].habilidades = cria_cjt(max_hab_missao)))
            return 0;
        for (j = 0; j < max_hab_missao; j++)
            insere_cjt(missoes[i].habilidades, ALEAT(0, N_HABILIDADES - 1));
    }

    return 1;
}

/* passa o numero pelo mesmo motivo de destroi_herois e bases */
void destroi_missoes(struct Missao missoes[], int n)
{
    int i;
    for (i = 0; i < n; i++) {
        if (missoes[i].habilidades != NULL)
            destroi_cjt(missoes[i].habilidades);
    }
}

int cria_eventos_iniciais(struct lef_t *eventos)
{
    int i;
    struct evento_t *e;
    for (i = 0; i < N_HEROIS; i++) {
        if (!(e = cria_evento(ALEAT(0, MAX_TEMPO_CHEGADA),
                              EV_CHEGA,
                              i,
                              ALEAT(0, N_BASES - 1))))
            return 0;
        insere_lef(eventos, e);
    }

    for (i = 0; i < N_MISSOES; i++) {
        if (!(e = cria_evento(ALEAT(0, FIM_DO_MUNDO), EV_MISSAO, i, 0)))
            return 0;
        insere_lef(eventos, e);
    }

    if (!(e = cria_evento(FIM_DO_MUNDO, EV_FIM_DO_MUNDO, 0, 0)))
        return 0;
    insere_lef(eventos, e);

    return 1;
}

/* EV_CHEGA 0
 * EV_ESPERA 1
 * EV_DESISTE 2
 * EV_AVISA 3
 * EV_ENTRA 4
 * EV_SAI 5
 * EV_VIAJA 6
 * EV_MISSAO 7 */
int loop_de_eventos(struct Mundo *mundo)
{
    struct evento_t *e;
    while ((e = retira_lef(mundo->eventos))) {
        switch (e->tipo) {
        case EV_FIM_DO_MUNDO:
            return 1;
            break;
        case EV_CHEGA:
            printf("%6d: CHEGA  HEROI %2d BASE %d (%d/%d) <todo>\n",
                   e->tempo,
                   e->dado1,
                   e->dado2,
                   cardinalidade_cjt(mundo->bases[e->dado2].herois),
                   mundo->bases[e->dado2].lotacao);
            break;
        case EV_ESPERA:
            printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n",
                   e->tempo,
                   e->dado1,
                   e->dado2,
                   fila_tamanho(mundo->bases[e->dado2].espera));
            break;
        case EV_DESISTE:
            printf("%6d: DESIST HEROI %2d BASE %d (%2d)\n",
                   e->tempo,
                   e->dado1,
                   e->dado2,
                   fila_tamanho(mundo->bases[e->dado2].espera));
            break;
        case EV_AVISA:
            printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ",
                   e->tempo,
                   e->dado1,
                   cardinalidade_cjt(mundo->bases[e->dado1].herois),
                   mundo->bases[e->dado1].lotacao);
            fila_imprime(mundo->bases[e->dado1].espera);
            break;
        case EV_ENTRA:
            printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI <todo>\n",
                   e->tempo,
                   e->dado1,
                   e->dado2,
                   cardinalidade_cjt(mundo->bases[e->dado2].herois),
                   mundo->bases[e->dado2].lotacao);
            break;
        case EV_SAI:
            printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n",
                   e->tempo,
                   e->dado1,
                   e->dado2,
                   cardinalidade_cjt(mundo->bases[e->dado1].herois),
                   mundo->bases[e->dado2].lotacao);
            break;
        case EV_VIAJA:
            printf("%6d: VIAJA  HEROI %2d BASE %2d BASE %2d DIST <todo> VEL %d CHEGA <todo>\n",
                   e->tempo,
                   e->dado1,
                   mundo->herois[e->dado1].base_id,
                   e->dado2,
                   mundo->herois[e->dado1].velocidade);
            break;
        case EV_MISSAO:
            printf("%6d: MISSAO %d <todo>\n", e->tempo, e->dado1);
        }
    }

    return 0;
}

static struct Mundo mundo;

int main()
{
    struct evento_t *e;
    mundo.tamanho.x = 20000;
    mundo.tamanho.y = 20000;

    srand(0);

    if (!inicializa_missoes(mundo.missoes, mundo.tamanho.x, mundo.tamanho.y)) {
        fprintf(stderr, "Falha inicializando missoes.\n");
        return 1;
    }

    if (!inicializa_herois(mundo.herois)) {
        fprintf(stderr, "Falha inicializando herois.\n");
        return 1;
    }

    if (!inicializa_bases(mundo.bases, mundo.tamanho.x, mundo.tamanho.y)) {
        fprintf(stderr, "Falha inicializando bases.\n");
        return 1;
    }

    if (!(mundo.eventos = cria_lef())) {
        fprintf(stderr, "Falha ao criar lef para eventos.\n");
        return 1;
    }

    if (!cria_eventos_iniciais(mundo.eventos)) {
        fprintf(stderr, "Falha ao criar eventos iniciais.\n");
        return 1;
    }

    if (!loop_de_eventos(&mundo)) {
        fprintf(stderr, "Eventos acabaram antes do fim do mundo.\n");
        return 1;
    }

    destroi_missoes(mundo.missoes, N_MISSOES);
    destroi_herois(mundo.herois, N_HEROIS);
    destroi_bases(mundo.bases, N_BASES);

    while ((e = retira_lef(mundo.eventos)))
        free(e);
    destroi_lef(mundo.eventos);

    return 0;
}
