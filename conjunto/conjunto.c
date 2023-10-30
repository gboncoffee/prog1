#include <stdlib.h>
#include <stdio.h>
#include "conjunto.h"

/* os conjuntos sao mantidos ordenados pois isso deixa varias operacoes mais
 * baratas por meio da busca binaria */

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct conjunto *cria_cjt(int max)
{
    struct conjunto *novo;

    if (!(novo = malloc(sizeof(struct conjunto))))
        return NULL;
    if (!(novo->v = malloc(sizeof(int) * max))) {
        free(novo);
        return NULL;
    }

    novo->max = max;
    novo->card = 0;
    novo->ptr = 0;

    return novo;
}

struct conjunto *destroi_cjt(struct conjunto *c)
{
    if (c == NULL)
        return NULL;
    if (c->v != NULL)
        free(c->v);
    free(c);

    return NULL;
}

int vazio_cjt(struct conjunto *c)
{
    if (c == NULL)
        return 1;
    return c->card == 0;
}

int cardinalidade_cjt(struct conjunto *c)
{
    if (c == NULL)
        return 0;
    return c->card;
}

/* serve para varias operacoes, retorna a posicao em pos. o valor de retorno eh
 * 1 em sucesso. nao verifica NULL, o chamador deve verificar
 *
 * pos nao pode ser NULL de jeito nenhum pois sempre mexe-se nela */
int busca_binaria(struct conjunto *c, int elemento, int ini, int fim, int *pos)
{
    /* precisamos garantir que *pos tera a posicao exata em que precisamos 
     * inserir um elemento se necessario */
    if (ini > fim) {
        *pos = ini;
        return 0;
    }
    if (ini == fim) {
        if (c->v[ini] == elemento) {
            *pos = ini;
            return 1;
        }
        if (c->v[ini] > elemento)
            *pos = ini;
        else
            *pos = ini + 1;
        return 0;
    }

    *pos = (fim - ini) / 2 + ini;
    if (c->v[*pos] > elemento)
        return busca_binaria(c, elemento, ini, *pos - 1, pos);
    if (c->v[*pos] < elemento)
        return busca_binaria(c, elemento, *pos + 1, fim, pos);
    return 1;
}

#define BUSCA(c, e, pos) busca_binaria((c), (e), 0, (c)->card - 1, (pos))

int pertence_cjt(struct conjunto *c, int elemento)
{
    if (c == NULL)
        return 0;
    int pos;
    return BUSCA(c, elemento, &pos);
}

/* auxiliar para insere_cjt. mov deve comecar como a ultima posicao possivel */
void insere_pos(struct conjunto *c, int elemento, int pos, int mov)
{
    if (mov < pos) {
        c->v[pos] = elemento;
        return;
    }

    c->v[mov + 1] = c->v[mov];
    insere_pos(c, elemento, pos, mov - 1);
}

int insere_cjt(struct conjunto *c, int elemento)
{
    int pos = 0;
    if (c == NULL || c->card == c->max)
        return 0;
    if (BUSCA(c, elemento, &pos))
        return 1;

    insere_pos(c, elemento, pos, c->card - 1);
    c->card++;

    return 1;
}

/* auxiliar para retira_cjt semelhante a insere_pos */
void retira_pos(struct conjunto *c, int pos, int fim)
{
    if (pos >= fim)
        return;
    c->v[pos] = c->v[pos + 1];
    retira_pos(c, pos + 1, fim);
}

int retira_cjt(struct conjunto *c, int elemento)
{
    int pos;
    if (c == NULL || c->v == NULL || !BUSCA(c, elemento, &pos))
        return 0;

    c->card--;
    retira_pos(c, pos, c->card);
    return 1;
}

int contido(struct conjunto *c1, struct conjunto *c2, int p1, int p2)
{
    if (p1 == c1->card)
        return 1;
    if (p2 == c2->card)
        return 0;

    if (c1->v[p1] == c2->v[p2])
        return contido(c1, c2, p1 + 1, p2 + 1);
    if (c1->v[p1] < c2->v[p2])
        return 0;
    return contido(c1, c2, p1, p2 + 1);
}

int contido_cjt(struct conjunto *c1, struct conjunto *c2)
{
    if (c1 == NULL || c2 == NULL)
        return 0;
    return contido(c1, c2, 0, 0);
}

/* compara dois conjuntos de mesma cardinalidade, auxiliar para
 * sao_iguais_cjt */
int cmpcjt(struct conjunto *c1, struct conjunto *c2, int p)
{
    if (c1->card == p)
        return 1;
    if (c1->v[p] != c2->v[p])
        return 0;
    return cmpcjt(c1, c2, p + 1);
}

int sao_iguais_cjt(struct conjunto *c1, struct conjunto *c2)
{
    if (c1 == NULL || c2 == NULL || c1->card != c2->card)
        return 0;
    return cmpcjt(c1, c2, 0);
}

/* os algoritmos que computam diferenca, uniao e interseccao sao bem parecidos.
 * ambos iteram os dois conjuntos aumentando o ponteiro de cada um quando o
 * elemento do outro for maior. */
void computa_diff(struct conjunto *c1,
                  struct conjunto *c2,
                  struct conjunto *cr,
                  int p1,
                  int p2)
{
    int novo_p1 = p1;
    int novo_p2 = p2;

    if (p1 >= c1->card)
        return;

    if (p2 >= c2->card || c1->v[p1] < c2->v[p2]) {
        cr->v[cr->card] = c1->v[p1];
        cr->card++;
        novo_p1++;
    } else {
        if (c1->v[p1] == c2->v[p2])
            novo_p1++;
        novo_p2++;
    }

    computa_diff(c1, c2, cr, novo_p1, novo_p2);
}

struct conjunto *diferenca_cjt(struct conjunto *c1, struct conjunto *c2)
{
    struct conjunto *diff = cria_cjt(c1->card);
    if (diff == NULL)
        return NULL;
    computa_diff(c1, c2, diff, 0, 0);
    return diff;
}

void computa_intc(struct conjunto *c1,
                  struct conjunto *c2,
                  struct conjunto *cr,
                  int p1,
                  int p2)
{
    int novo_p2 = p2;
    int novo_p1 = p1;
    if (p1 >= c1->card || p2 >= c2->card)
        return;

    if (c1->v[p1] == c2->v[p2]) {
        cr->v[cr->card] = c1->v[p1];
        cr->card++;
    }

    if (c1->v[p1] >= c2->v[p2])
        novo_p2++;
    if (c1->v[p1] <= c2->v[p2])
        novo_p1++;

    computa_intc(c1, c2, cr, novo_p1, novo_p2);
}

struct conjunto *interseccao_cjt(struct conjunto *c1, struct conjunto *c2)
{
    struct conjunto *intc = cria_cjt(MIN(c1->card, c2->card));
    if (intc == NULL)
        return NULL;
    computa_intc(c1, c2, intc, 0, 0);
    return intc;
}

void computa_uniao(struct conjunto *c1,
                   struct conjunto *c2,
                   struct conjunto *cr,
                   int p1,
                   int p2)
{
    if (p1 >= c1->card && p2 >= c2->card)
        return;

    if (p1 >= c1->card || (c1->v[p1] > c2->v[p2])) {
        cr->v[cr->card] = c2->v[p2];
        p2++;
    } else if (p2 >= c2->card || (c2->v[p2] > c1->v[p1])) {
        cr->v[cr->card] = c1->v[p1];
        p1++;
    } else {
        cr->v[cr->card] = c1->v[p1];
        p1++;
        p2++;
    }

    cr->card++;
    computa_uniao(c1, c2, cr, p1, p2);
}

struct conjunto *uniao_cjt(struct conjunto *c1, struct conjunto *c2)
{
    struct conjunto *uniao = cria_cjt(c1->card + c2->card);
    if (uniao == NULL)
        return NULL;
    computa_uniao(c1, c2, uniao, 0, 0);
    return uniao;
}

void copia(struct conjunto *dest, struct conjunto *src, int idx)
{
    if (idx == src->card)
        return;
    dest->v[idx] = src->v[idx];
    copia(dest, src, idx + 1);
}

struct conjunto *copia_cjt(struct conjunto *c)
{
    struct conjunto *novo = cria_cjt(c->card);
    if (novo == NULL)
        return NULL;
    copia(novo, c, 0);
    novo->card = c->card;

    return novo;
}

int retira_um_elemento_cjt(struct conjunto *c)
{
    int r, n;
    if (vazio_cjt(c))
        return 0;
    if (c->card == 1)
        r = 0;
    else
        r = rand() % (c->card - 1);
    n = c->v[r];
    retira_pos(c, r, c->card - 1);
    c->card--;
    return n;
}

struct conjunto *cria_subcjt_cjt(struct conjunto *c, int n)
{
    int i;
    struct conjunto *copia;
    struct conjunto *novo;

    if (n == 0 || vazio_cjt(c))
        return cria_cjt(c->max);

    copia = copia_cjt(c);
    if (n >= c->card)
        return copia;

    novo = cria_cjt(c->card);
    for (i = 0; i < n; i++)
        insere_cjt(novo, retira_um_elemento_cjt(copia));

    destroi_cjt(copia);

    return novo;
}

void imprime_cjt(struct conjunto *c)
{
    int i;
    if (c->card == 0) {
        printf("conjunto vazio\n");
        return;
    }
    for (i = 0; i < c->card - 1; i++)
        printf("%d ", c->v[i]);
    printf("%d\n", c->v[c->card - 1]);
}

void inicia_iterador_cjt(struct conjunto *c)
{
    c->ptr = 0;
}

int incrementa_iterador_cjt(struct conjunto *c, int *ret_iterador)
{
    if (c->ptr >= c->card)
        return 0;
    *ret_iterador = c->v[c->ptr];
    c->ptr++;
    return 1;
}
