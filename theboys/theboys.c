#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include "conjunto.h"
#include "fila.h"
#include "lef.h"

/* todas as funcoes que retornam int para indicar falha retornam 1 em sucesso e
 * 0 em falha */

/* configuracao da simulacao */
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
#define MIN_HAB_HEROI 1
#define MAX_HAB_HEROI 3
#define MIN_HAB_MISSAO 6
#define MAX_HAB_MISSAO
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

#define ALEAT(a, b) ((rand() % ((b - a) + 1)) + (a))

struct Coordenada {
    int x;
    int y;
};

int dist_cartesiana(struct Coordenada a, struct Coordenada b)
{
    float diff_x = (float) a.x - b.x;
    float diff_y = (float) a.y - b.y;
    return (int) sqrt(powf(diff_x, 2) + powf(diff_y, 2));
}

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
    int tentativas;
    short int completa;
};

struct Mundo {
    struct Base bases[N_BASES];
    struct Heroi herois[N_HEROIS];
    struct Missao missoes[N_MISSOES];
    struct lef_t *eventos;
    struct Coordenada tamanho;
};

/* eventos */

#define NOVO_EVENTO(mundo, tipo, tempo, dado1, dado2) {\
        struct evento_t *n = cria_evento((tempo), (tipo), (dado1), (dado2));\
        insere_lef((mundo)->eventos, n);\
    }

int chega(struct Mundo *mundo, int tempo, int heroi, int base)
{
    struct Base *b = &mundo->bases[base];
    struct Heroi *h = &mundo->herois[heroi];

    h->base_id = base;

    if ((cardinalidade_cjt(b->herois) < b->lotacao && fila_vazia(b->espera)) ||
        (h->paciencia > (10 * fila_tamanho(b->espera))))
    {
        NOVO_EVENTO(mundo, EV_ESPERA, tempo, heroi, base);
        return 1;
    }

    NOVO_EVENTO(mundo, EV_DESISTE, tempo, heroi, base);

    return 0;
}

void avisa(struct Mundo *mundo, int tempo, int base)
{
    struct Base *b = &mundo->bases[base];
    int heroi;

    while (cardinalidade_cjt(b->herois) < b->lotacao &&
        dequeue(b->espera, &heroi))
    {
        insere_cjt(b->herois, heroi);
        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", tempo, base, heroi);
        NOVO_EVENTO(mundo, EV_ENTRA, tempo, heroi, base);
    }
}

int entra(struct Mundo *mundo, int heroi, int base, int tempo)
{
    struct Heroi *h = &mundo->herois[heroi];
    int t_saida = tempo + 15 + h->paciencia * ALEAT(1, 20);
    NOVO_EVENTO(mundo, EV_SAI, t_saida, heroi, base);

    return t_saida;
}

void sai(struct Mundo *mundo, int heroi, int base, int tempo)
{
    int dest = ALEAT(0, N_BASES - 1);
    retira_cjt(mundo->bases[base].herois, heroi);
    NOVO_EVENTO(mundo, EV_VIAJA, tempo, heroi, dest);
    NOVO_EVENTO(mundo, EV_AVISA, tempo, base, 0);
}

void viaja(struct Mundo *mundo, int heroi, int base, int tempo)
{
    struct Heroi *h = &mundo->herois[heroi];
    struct Base *b = &mundo->bases[base];
    int dist = dist_cartesiana(mundo->bases[h->base_id].local, b->local);
    int chega = tempo + dist / h->velocidade;
    printf("%6d: VIAJA  HEROI %2d BASE %2d BASE %2d DIST %d VEL %d CHEGA %d\n",
           tempo,
           heroi,
           h->base_id,
           base,
           dist,
           h->velocidade,
           chega);
    NOVO_EVENTO(mundo, EV_CHEGA, chega, heroi, base);
}

/* auxiliar para missao */
struct conjunto *une_habilidades_base(struct Heroi *herois, struct Base *b)
{
    int h, hn;
    struct conjunto *u, *tmp;

    inicia_iterador_cjt(b->herois);
    if (incrementa_iterador_cjt(b->herois, &h) == 0)
        return cria_cjt(1);
    if (incrementa_iterador_cjt(b->herois, &hn) == 0)
        return copia_cjt(herois[h].habilidades);

    u = uniao_cjt(herois[h].habilidades, herois[hn].habilidades);
    while (incrementa_iterador_cjt(b->herois, &h)) {
        tmp = uniao_cjt(u, herois[h].habilidades);
        destroi_cjt(u);
        u = tmp;
    }

    return u;
}

/* roda quando a missao pode ser completa */
void completa(struct Mundo *mundo, int missao, int base)
{
    int i;
    mundo->missoes[missao].completa = 1;

    inicia_iterador_cjt(mundo->bases[base].herois);
    while (incrementa_iterador_cjt(mundo->bases[base].herois, &i))
        mundo->herois[i].experiencia++;
}

/* struct usada so em missao para ordenar as bases pela distancia */
struct BaseDistancia {
    int base;
    int dist;
};

void missao(struct Mundo *mundo, int missao, int tempo)
{
    struct BaseDistancia bases[N_BASES];
    struct BaseDistancia ult;
    int i, j;
    struct conjunto *uniao;
    struct Missao *m = &mundo->missoes[missao];

    m->tentativas++;
    printf("%6d: MISSAO %d HAB REQ: ", tempo, missao);
    imprime_cjt(m->habilidades);

    for (i = 0; i < N_BASES; i++) {
        bases[i] = (struct BaseDistancia) {
            .base = i,
            .dist = dist_cartesiana(mundo->bases[i].local, m->local),
        };
    }

    /* insertion sort */
    for (i = 1; i < N_BASES; i++) {
        ult = bases[i];
        j = i - 1;
        while (ult.dist < bases[j].dist && j >= 0) {
            bases[j + 1] = bases[j];
            j--;
        }
        bases[j + 1] = ult;
    }

    for (i = 0; i < N_BASES; i++) {
        uniao = une_habilidades_base(mundo->herois,
                                     &mundo->bases[bases[i].base]);
        if (uniao != NULL) {
            printf("%6d: MISSAO %d HAB BASE %d: ",
                   tempo,
                   missao,
                   bases[i].base);
            imprime_cjt(uniao);

            if (contido_cjt(m->habilidades, uniao)) {
                printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS: ",
                       tempo,
                       missao,
                       bases[i].base);
                imprime_cjt(mundo->bases[bases[i].base].herois);
                completa(mundo, missao, bases[i].base);
                return;
            }
        }
    }

    printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, missao);
    NOVO_EVENTO(mundo, EV_MISSAO, tempo + 24 * 60, missao, 0);
}

/* outras funcoes */

int loop_de_eventos(struct Mundo *mundo)
{
    struct evento_t *e;
    while ((e = retira_lef(mundo->eventos))) {
        switch (e->tipo) {
        case EV_FIM_DO_MUNDO:
            printf("%6d: FIM\n", e->tempo);
            free(e);
            return 1;
        case EV_CHEGA:
            printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ",
                   e->tempo,
                   e->dado1,
                   e->dado2,
                   cardinalidade_cjt(mundo->bases[e->dado2].herois),
                   mundo->bases[e->dado2].lotacao);
            if (chega(mundo, e->tempo, e->dado1, e->dado2))
                printf("ESPERA\n");
            else
                printf("DESISTE\n");

            break;
        case EV_ESPERA:
            printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n",
                   e->tempo,
                   e->dado1,
                   e->dado2,
                   fila_tamanho(mundo->bases[e->dado2].espera));
            enqueue(mundo->bases[e->dado2].espera, e->dado1);
            NOVO_EVENTO(mundo, EV_AVISA, e->tempo, e->dado2, 0);

            break;
        case EV_DESISTE:
            printf("%6d: DESIST HEROI %2d BASE %d (%2d)\n",
                   e->tempo,
                   e->dado1,
                   e->dado2,
                   fila_tamanho(mundo->bases[e->dado2].espera));
            NOVO_EVENTO(mundo,
                        EV_VIAJA,
                        e->tempo,
                        e->dado1,
                        ALEAT(0, N_BASES - 1));

            break;
        case EV_AVISA:
            printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ",
                   e->tempo,
                   e->dado1,
                   cardinalidade_cjt(mundo->bases[e->dado1].herois),
                   mundo->bases[e->dado1].lotacao);
            fila_imprime(mundo->bases[e->dado1].espera);
            printf("\n");
            avisa(mundo, e->tempo, e->dado1);

            break;
        case EV_ENTRA:
            printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n",
                   e->tempo,
                   e->dado1,
                   e->dado2,
                   cardinalidade_cjt(mundo->bases[e->dado2].herois),
                   mundo->bases[e->dado2].lotacao,
                   entra(mundo, e->dado1, e->dado2, e->tempo));
            break;
        case EV_SAI:
            printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n",
                   e->tempo,
                   e->dado1,
                   e->dado2,
                   cardinalidade_cjt(mundo->bases[e->dado2].herois),
                   mundo->bases[e->dado2].lotacao);
            sai(mundo, e->dado1, e->dado2, e->tempo);
            break;
        case EV_VIAJA:
            viaja(mundo, e->dado1, e->dado2, e->tempo);
            break;
        case EV_MISSAO:
            missao(mundo, e->dado1, e->tempo);
            break;
        }

        free(e);
    }

    return 0;
}

int inicializa_herois(struct Heroi herois[])
{
    int i, j, max_hab_heroi;
    for (i = 0; i < N_HEROIS; i++) {
        herois[i].base_id = BASE_INVALIDA;
        herois[i].experiencia = 0;
        herois[i].paciencia = ALEAT(MIN_PACIENCIA, MAX_PACIENCIA);
        herois[i].velocidade = ALEAT(MIN_VELOCIDADE, MAX_VELOCIDADE);
        max_hab_heroi = ALEAT(MIN_HAB_HEROI, MAX_HAB_HEROI);
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

int inicializa_missoes(struct Missao *missoes, int maxx, int maxy)
{
    int i, j, max_hab_missao;
    for (i = 0; i < N_MISSOES; i++) {
        missoes[i].local.x = ALEAT(0, maxx);
        missoes[i].local.y = ALEAT(0, maxy);
        missoes[i].completa = 0;
        missoes[i].tentativas = 0;
        max_hab_missao = ALEAT(MIN_HAB_MISSAO, MAX_HAB_MISSAO);
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

void relatorio(struct Mundo *mundo)
{
    int i;
    struct Heroi *h;
    struct Missao *m;
    int completas = 0;
    int tentativas = 0;
    for (i = 0; i < N_HEROIS; i++) {
        h = &mundo->herois[i];
        printf("HEROI %2d PAC %3d VEL %4d EXP %4d HAB ",
               i,
               h->paciencia,
               h->velocidade,
               h->experiencia);
        imprime_cjt(h->habilidades);
    }

    for (i = 0; i < N_MISSOES; i++) {
        m = &mundo->missoes[i];
        completas += m->completa;
        tentativas += m->tentativas;
    }

    printf("%d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n",
           completas,
           N_MISSOES,
           (float) completas * 100 / N_MISSOES,
           (float) tentativas / N_MISSOES);
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

    relatorio(&mundo);

    destroi_missoes(mundo.missoes, N_MISSOES);
    destroi_herois(mundo.herois, N_HEROIS);
    destroi_bases(mundo.bases, N_BASES);

    while ((e = retira_lef(mundo.eventos)))
        free(e);
    destroi_lef(mundo.eventos);

    return 0;
}
