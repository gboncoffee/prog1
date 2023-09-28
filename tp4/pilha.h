/*
 * TAD pilha
 * -> versao com lista ligada com nodo cabeca
 * Autores:
 *    Andre Ricardo Abed Gregio
 *    Marcos Alexandre Castilho
 *    Luis Carlos Erpen de Bona
 *
 * Versao 2.0.0 de 25/09/2023
*/

#ifndef _pilha_t_H
#define _pilha_t_H

struct nodo {
    int chave;
    struct nodo *prox;
};

struct pilha {
    struct nodo *topo;
    int tamanho;
};

/* 
 * Cria e retorna uma nova pilha.
 * Retorna NULL em caso de erro de alocação.
*/
struct pilha *pilha_cria ();

/* Desaloca toda memoria da pilha e faz pilha receber NULL. */
void pilha_destroi (struct pilha **pilha);

/* 
 * Insere dado na pilha (politica LIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int push (struct pilha *pilha, int dado);

/* 
 * Remove o topo (politica LIFO) e retorna o elemento 
 * no parametro dado. A funcao retorna 1 em caso de 
 * sucesso e 0 no caso da pilha estar vazia.
*/
int pop (struct pilha *pilha, int *dado);
 
/* 
 * Similar ao pop, mas retorna o elemento dado sem remove-lo. 
 * A funcao retorna 1 em caso de sucesso e 0 no caso 
 * da pilha estar vazia.
*/
int pilha_topo (struct pilha *pilha, int *dado); 

/* Retorna o numero de elementos da pilha, que pode ser 0. */
int pilha_tamanho (struct pilha *pilha);

/* Retorna 1 se pilha vazia, 0 em caso contrario. */ 
int pilha_vazia (struct pilha *pilha);

#endif
