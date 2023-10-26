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

struct nodo_lef_t *cria_nodo_lef(struct evento_t *evento, struct nodo_lef_t *prox)
{
    struct nodo_lef_t *novo;
    if (!(novo = malloc(sizeof(struct nodo_lef_t))))
        return NULL;

    novo->evento = evento;
    novo->prox = prox;

    return novo;
}

void destroi_nodos_lef(struct nodo_lef_t *nodo)
{
    if (nodo) {
        destroi_nodos_lef(nodo->prox);
        free(nodo);
    }
}

struct lef_t *destroi_lef(struct lef_t *l)
{
    if (l) {
        destroi_nodos_lef(l->primeiro);
        free(l);
    }

    return NULL;
}

int insere_nodo_lefs(struct nodo_lef_t *nodo, struct evento_t *e)
{
    if (nodo->prox && ((nodo->prox)->evento)->tempo <= e->tempo)
        return insere_nodo_lefs(nodo->prox, e);

    return (!(nodo->prox = cria_nodo_lef(e, nodo->prox)));
}

int insere_lef(struct lef_t *l, struct evento_t *e)
{
    if (!(l && e))
        return 0;

    if (!l->primeiro) {
        l->primeiro = cria_nodo_lef(e, NULL);
        return 1;
    }

    return insere_nodo_lefs(l->primeiro, e);
}

struct evento_t *retira_lef(struct lef_t *l)
{
    struct evento_t *e;
    struct nodo_lef_t *aux;
    if (!(l && l->primeiro))
        return NULL;
    e = (l->primeiro)->evento;
    aux = (l->primeiro)->prox;
    free(l->primeiro);
    l->primeiro = aux;

    return e;
}

int vazia_lef(struct lef_t *l)
{
    return (!(l && l->primeiro));
}

int imprime_nodos(struct nodo_lef_t *nodo, int n)
{
    struct evento_t *e;
    if (!(nodo && nodo->evento))
        return n;
    e = nodo->evento;

    printf("  tempo %d tipo %d d1 %d d2 %d\n", e->tempo, e->tipo, e->dado1, e->dado2);
    return imprime_nodos(nodo->prox, n + 1);
}

void imprime_lef(struct lef_t *l)
{
    int n = 0;
    printf("LEF:\n");
    if (l)
        n = imprime_nodos(l->primeiro, 0);
    printf("  Total %d eventos\n", n);
}
