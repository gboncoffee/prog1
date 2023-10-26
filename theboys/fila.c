#include <stdlib.h>
#include "fila.h"

struct nodo *cria_nodo_fila(int dado, struct nodo *prox)
{
    struct nodo *novo;
    if (!(novo = malloc(sizeof(struct nodo))))
        return NULL;
    novo->chave = dado;
    novo->prox = prox;
    return novo;
}

void destroi_nodos_fila(struct nodo *nodo)
{
    if (nodo != NULL) {
        destroi_nodos_fila(nodo->prox);
        free(nodo);
    }
}

struct fila *fila_cria()
{
    struct fila *nova;
    if (!(nova = malloc(sizeof(struct fila))))
        return NULL;

    nova->ini = NULL;
    nova->fim = NULL;
    nova->tamanho = 0;

    return nova;
}

void fila_destroi(struct fila **fila)
{
    destroi_nodos_fila((*fila)->ini);
    free(*fila);
    *fila = NULL;
}

int enqueue(struct fila *fila, int dado)
{
    struct nodo *novo;
    if (!(novo = cria_nodo_fila(dado, NULL)))
        return 0;

    /* o tad garante que se fila->fim for NULL, ini tambem eh e a fila esta
     * vazia */
    if (fila->fim == NULL)
        (fila->ini) = novo;
    else
        (fila->fim)->prox = novo;

    fila->fim = novo;
    fila->tamanho++;

    return 1;
}

int dequeue(struct fila *fila, int *dado)
{
    struct nodo *aux;
    if (!fila->tamanho)
        return 0;
    *dado = (fila->ini)->chave;
    aux = (fila->ini)->prox;
    free(fila->ini);
    fila->ini = aux;
    fila->tamanho--;

    if (fila->ini == NULL)
        fila->fim = NULL;

    return 1;
}

int fila_tamanho(struct fila *fila)
{
    return fila->tamanho;
}

int fila_vazia(struct fila *fila)
{
    return fila->tamanho ? 0 : 1;
}
