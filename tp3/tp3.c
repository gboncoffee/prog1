#include <stdlib.h>
#include <stdio.h>
#include "racionais.h"

int inicia_vetor(struct racional **v, int n)
{
    int i, den, num;
    for (i = 0; i < n; i++) {
        scanf("%d", &num);
        scanf("%d", &den);
        v[i] = cria_r(num, den);
        if (v[i] == NULL)
            return 1;
    }

    return 0;
}

void imprime_vetor(struct racional **v, int n)
{
    if (n <= 0)
        return;
    int i;
    for (i = 0; i < n - 1; i++) {
        imprime_r(v[i]);
        printf(" ");
    }
    imprime_r(v[n - 1]);
}

void elimina_invalido(struct racional **v, int *n)
{
    int i = 0;
    while (i < *n) {
        if (!valido_r(v[i])) {
            destroi_r(v[i]);
            (*n)--;
            while ((*n) > i) {
                if (valido_r(v[*n])) {
                    v[i] = v[*n];
                    /* eu nao consegui tirar esse break daqui de maneira
                     * eficiente */
                    break;
                }
                destroi_r(v[*n]);
                (*n)--;
            }
        }

        i++;
    }
}

int pos_menor(struct racional **v, int n, int i)
{
    int r = i;
    for (; i < n; i++) {
        if (compara_r(v[i], v[r]) == -1)
            r = i;
    }

    return r;
}

void selection_sort(struct racional **v, int n, int i)
{
    int p;
    struct racional *tmp;
    if (n > i) {
        p = pos_menor(v, n, i);
        tmp = v[i];
        v[i] = v[p];
        v[p] = tmp;
        selection_sort(v, n, i + 1);
    }
}

struct racional *soma_vec_r(struct racional **v, int n, int i)
{
    struct racional *r;
    struct racional *prox;
    long int num, den;
    if (n <= 0) {
        return cria_r(0, 1);
    } else if (i == (n - 1)) {
        numerador_r(v[i], &num);
        denominador_r(v[i], &den);

        return cria_r(num, den);
    }

    prox = cria_r(0, 1);
    r = soma_vec_r(v, n, i + 1);

    soma_r(v[i], r, prox);
    simplifica_r(prox);

    destroi_r(r);

    return prox;
}

int main()
{
    /* vetor de ponteiros para racionais */
    struct racional **v;  /* equivalente a struct racional *v[] */
    struct racional *soma;
    int i;

    int n;
    scanf("%d", &n);

    v = (struct racional **) malloc(sizeof(struct racional *) * n);
    if (v == NULL)
        return 1;

    if (inicia_vetor(v, n))
        return 1;

    imprime_vetor(v, n);
    printf("\n");
    elimina_invalido(v, &n);
    imprime_vetor(v, n);
    printf("\n");
    selection_sort(v, n, 0);
    imprime_vetor(v, n);
    printf("\n");

    soma = soma_vec_r(v, n, 0);
    printf("a soma de todos os elementos eh ");
    imprime_r(soma);
    destroi_r(soma);
    printf("\n");

    for (i = 0; i < n; i++)
        destroi_r(v[i]);
    free(v);

    return 0;
}
