#include <stdlib.h>
#include <stdio.h>
#include "lef.h"

struct evento_t *cria_evento(int tempo, int tipo, int dado1, int dado2)
{
    struct evento_t *novo;
    if (!(novo = malloc(sizeof(struct evento_t))))
        return NULL;

    novo->tempo = tempo;
    novo->tipo = tipo;
    novo->dado1 = dado1;
    novo->dado2 = dado2;

    return novo;
}

struct evento_t *destroi_evento(struct evento_t *e)
{
    if (e)
        free(e);
    return NULL;
}

struct lef_t *cria_lef()
{
    struct lef_t *nova;
    if (!(nova = malloc(sizeof(struct lef_t))))
        return NULL;
    nova->primeiro = NULL;

    return nova;
}

struct nodo_lef_t *cria_nodo(struct evento_t *evento, struct nodo_lef_t *prox)
{
    struct nodo_lef_t *novo;
    if (!(novo = malloc(sizeof(struct nodo_lef_t))))
        return NULL;

    novo->evento = evento;
    novo->prox = prox;

    return novo;
}

void destroi_nodos(struct nodo_lef_t *nodo)
{
    if (nodo) {
        destroi_nodos(nodo->prox);
        free(nodo);
    }
}

struct lef_t *destroi_lef(struct lef_t *l)
{
    if (l) {
        destroi_nodos(l->primeiro);
        free(l);
    }

    return NULL;
}

int insere_nodo_lefs(struct nodo_lef_t *nodo, struct evento_t *e)
{
    struct nodo_lef_t *novo;
    if (nodo->prox && ((nodo->prox)->evento)->tempo >= e->tempo)
        return insere_nodo_lefs(nodo->prox, e);

    if (!(novo = cria_nodo(e, nodo->prox)))
        return 0;

    nodo->prox = novo;
    return 1;
}

int insere_lef(struct lef_t *l, struct evento_t *e)
{
    if (!(l && e))
        return 0;

    if (!l->primeiro) {
        l->primeiro = cria_nodo(e, NULL);
        return 1;
    }

    return insere_nodo_lefs(l->primeiro, e);
}

struct evento_t *retira_lef(struct lef_t *l)
{
    struct evento_t *e;
    if (!(l && l->primeiro))
        return NULL;
    e = (l->primeiro)->evento;
    free(l->primeiro);
    l->primeiro = (l->primeiro)->prox;

    return e;
}

int vazia_lef(struct lef_t *l)
{
    return (!(l && l->primeiro));
}

void imprime_nodos(struct nodo_lef_t *nodo)
{
    struct evento_t *e;
    if (!(nodo && nodo->evento))
        return;
    e = nodo->evento;

    printf("%d %d %d %d\n", e->tempo, e->tipo, e->dado1, e->dado2);
    imprime_nodos(nodo->prox);
}

void imprime_lef(struct lef_t *l)
{
    if (l)
        imprime_nodos(l->primeiro);
}
