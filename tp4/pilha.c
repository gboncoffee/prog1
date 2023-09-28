#include <stdlib.h>
#include "pilha.h"

struct nodo *cria_nodo(int dado, struct nodo *prox)
{
    struct nodo *novo;
    if (!(novo = malloc(sizeof(struct nodo))))
        return NULL;
    novo->chave = dado;
    novo->prox = prox;
    return novo;
}

void destroi_nodos(struct nodo *nodo)
{
    if (nodo != NULL) {
        destroi_nodos(nodo->prox);
        free(nodo);
    }
}

struct pilha *pilha_cria()
{
    struct pilha *nova;
    if (!(nova = malloc(sizeof(struct pilha))))
        return NULL;

    nova->topo = NULL;
    nova->tamanho = 0;

    return nova;
}

void pilha_destroi(struct pilha **pilha)
{
    destroi_nodos((*pilha)->topo);
    free(*pilha);
    *pilha = NULL;
}

int push(struct pilha *pilha, int dado)
{
    struct nodo *novo;
    if (!(novo = cria_nodo(dado, pilha->topo)))
        return 0;

    pilha->topo = novo;
    pilha->tamanho++;

    return 1;
}

int pop(struct pilha *pilha, int *dado)
{
    struct nodo *aux;
    if (!pilha->tamanho)
        return 0;
    *dado = (pilha->topo)->chave;
    aux = (pilha->topo)->prox;
    free(pilha->topo);
    pilha->topo = aux;
    pilha->tamanho--;

    return 1;
}

int pilha_topo(struct pilha *pilha, int *dado)
{
    if (!pilha->tamanho)
        return 0;
    *dado = (pilha->topo)->chave;

    return 1;
}

int pilha_tamanho(struct pilha *pilha)
{
    return pilha->tamanho;
}

int pilha_vazia(struct pilha *pilha)
{
    return pilha->tamanho ? 0 : 1;
}
