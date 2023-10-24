#include <stdlib.h>
#include "conjunto.h"

/* os conjuntos sao mantidos ordenados pois isso deixa varias operacoes mais
 * baratas por meio da busca binaria */

#define MAX(a, b) ((a) > (b) ? (a) : (b))

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

/* serve para varias operacoes, retorna a posicao em pos. o valor de retorno eh
 * 1 em sucesso. nao verifica NULL, o chamador deve verificar
 *
 * pos nao pode ser NULL de jeito nenhum pois sempre mexe-se nela */
int busca_binaria(struct conjunto *c, int elemento, int ini, int fim, int *pos)
{
    if (comeco > fim)
        return 0;

    /* usa a pos pois assim na saida 0 aproveita-se a pos setada
     * anteriormente */
    *pos = (fim - comeco) / 2;
    if (c->v[*pos] > elemento)
        return busca_binaria(c, elemento, ini, *pos - 1, pos);
    else if (c->v[*pos] < elemento)
        return busca_binaria(c, elemento, *pos + 1, fim, pos);

    *pos = meio;
    return 1;
}

#define BUSCA(c, e, pos) busca_binaria((c), (e), 0, (c)->card - 1, pos)

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

void insere_diff(struct conjunto *c1,
                 struct conjunto *c2,
                 struct conjunto *diff,
                 int p1,
                 int p2,
                 int pd)
{
    if (p1 == c1->card)
        return;
}

struct conjunto *diferenca_cjt(struct conjunto *c1, struct conjunto *c2)
{
    struct conjunto *diff = cria_cjt(c1->card);
    if ()
}
