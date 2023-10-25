#include <stdlib.h>
#include <stdio.h>
#include "conjunto.h"

/* os conjuntos sao mantidos ordenados pois isso deixa varias operacoes mais
 * baratas por meio da busca binaria */

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct conjunto *cria_cjt(int max)
{
    int *v;
    struct conjunto *novo;

    /* colocar "v = ..." embaixo continua aproveitando o shortcut e fica mais
     * legível por não ficar longo */
    v = malloc(sizeof(int) * max);
    if (!(v && (novo = malloc(sizeof(struct conjunto)))))
        return NULL;

    novo->v = v;
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

/* TODO: arrumar busca */

/* serve para varias operacoes, retorna a posicao em pos. o valor de retorno eh
 * 1 em sucesso. nao verifica NULL, o chamador deve verificar
 *
 * pos nao pode ser NULL de jeito nenhum pois sempre mexe-se nela */
int busca_binaria(struct conjunto *c, int elemento, int ini, int fim, int *pos)
{
    if (ini > fim)
        return 0;
    if (ini == fim) {
        *pos = ini;
        return c->v[ini] == elemento;
    }

    /* usa a pos pois assim na saida 0 aproveita-se a pos setada
     * anteriormente */
    *pos = (fim - ini) / 2 + 1;
    if (c->v[*pos] > elemento)
        return busca_binaria(c, elemento, ini, *pos - 1, pos);
    else if (c->v[*pos] < elemento)
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
    int pos;
    if (c == NULL || c->card == c->max)
        return 0;
    if (BUSCA(c, elemento, &pos))
        return 1;

    insere_pos(c, elemento, pos, c->card - 1);
    c->card++;

    return 1;
}

/* auxiliar para retira_cjt semelhante a insere_pos */
void retira_pos(struct conjunto *c, int pos, int mov)
{
    if (pos == mov)
        return;
    c->v[mov - 1] = c->v[mov];
    retira_pos(c, pos, mov - 1);
}

int retira_cjt(struct conjunto *c, int elemento)
{
    int pos;
    if (c == NULL || c->v == NULL || !BUSCA(c, elemento, &pos))
        return 0;

    c->card--;
    retira_pos(c, pos, c->card - 1);
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

/* compara dois conjuntos de mesma cardinalidade, auxiliar para sao_iguais_cjt */
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

/* basicamente, p1 e p2 sao indices em c1 e c2 que demonstram qual elemento
 * devemos computar. se c2[p2] > c1[p1], significa que nao ha c1[p1] em c2 e
 * podemos adiciona-lo ao resultado. isso so eh possivel pois os conjuntos estao
 * ordenados.
 *
 * se c2[p2] < c1[p1], precisamos continuar verificando em c2 e ainda nao
 * podemos adicionar c1[p1] ao resultado. se sao iguais, passamos para o proximo
 * elemento de cada. */
void computa_diff(struct conjunto *c1,
                  struct conjunto *c2,
                  struct conjunto *cr,
                  int p1,
                  int p2)
{
    if (p1 == c1->card)
        return;

    if (p2 < c2->card) {
        /* nao ha c1[p1] em c2 entao adicionamos ao resultado */
        if (c2->v[p2] > c1->v[p1]) {
            cr->v[cr->card] = c1->v[p1];
            cr->card++;
        }

        if (c2->v[p2] >= c1->v[p1])
            p1++;
        if (c2->v[p2] <= c1->v[p1])
            p2++;
    } else {
        /* so roda quando chegar ao final de c2, basicamente copia tudo que
         * resta de c1 para o resultado */
        cr->v[cr->card] = c1->v[p1];
        p1++;
        cr->card++;
    }
    computa_diff(c1, c2, cr, p1, p2);
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
    if (p1 >= c1->card || p2 >= c2->card)
        return;
    if (c1->v[p1] > c2->v[p2])
        p2++;
    if (c1->v[p1] < c2->v[p2])
        p1++;
    if (c1->v[p1] == c2->v[p2]) {
        cr->v[cr->card] = c1->v[p1];
        cr->card++;
    }
    computa_intc(c1, c2, cr, p1, p2);
}

struct conjunto *interseccao_cjt(struct conjunto *c1, struct conjunto *c2)
{
    struct conjunto *intc = cria_cjt(MIN(c1->card, c2->card));
    if (intc == NULL)
        return NULL;
    computa_intc(c1, c2, intc, 0, 0);
    return intc;
}

/* bem parecido com o diff
 *
 * adiciona c1[p1] caso seja menor ou igual a c2[p2], incrementa ambos caso
 * sejam <= o outro */
void computa_uniao(struct conjunto *c1,
                   struct conjunto *c2,
                   struct conjunto *cr,
                   int p1,
                   int p2)
{
    if (p1 >= c1->card && p2 >= c2->card)
        return;
    if (p1 < c1->card && p2 < c2->card) {
        if (c1->v[p1] > c2->v[p2]) {
            cr->v[cr->card] = c2->v[p2];
        } else {
            cr->v[cr->card] = c1->v[p1];
            p1++;
        }

        if (c1->v[p1] >= c2->v[p2])
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
    return novo;
}

int retira_um_elemento_cjt(struct conjunto *c)
{
    int r = rand() % c->card;
    int n = c->v[r];
    retira_pos(c, r, c->card - 1);
    c->card--;
    return n;
}

struct conjunto *cria_subcjt_cjt(struct conjunto *c, int n)
{
    struct conjunto *novo;
    int i;
    if (vazio_cjt(c))
        return cria_cjt(n);
    if (n >= c->card)
        return copia_cjt(c);

    novo = cria_cjt(n);

    for (i = 0; i < n; i++)
        insere_cjt(novo, retira_um_elemento_cjt(c));

    return novo;
}

void imprime_cjt(struct conjunto *c)
{
    int i;
    for (i = 0; i < c->card - 2; i++)
        printf("%d ", c->v[i]);
    printf("%d\n", c->v[c->card - 1]);
}

void inicia_iterador_cjt(struct conjunto *c)
{
    c->ptr = 0;
}

int incrementa_iterador_cjt(struct conjunto *c, int *ret_iterador)
{
    *ret_iterador = c->v[c->ptr];
    c->ptr++;
    return c->ptr < c->card;
}
