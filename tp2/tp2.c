#include <stdlib.h>
#include <stdio.h>
#include "racionais.h"

#define VEC_SIZE 100
#define SORT_MAX 10

#define IMPRIME_VEC_R(v, s) {			\
	int i;					\
	for (i = 0; i < (s - 1); i++) {		\
	    imprime_r(v[i]);			\
	    printf(" ");			\
	}					\
	imprime_r(v[s - 1]);			\
    }

void elimina_invalido(struct racional v[], int *n)
{
    int i = 0;
    while (i < *n) {
	if (!valido_r(v[i])) {
	    while ((*n) - 1 > i) {
		(*n)--;
		if (valido_r(v[*n])) {
		    v[i] = v[*n];
		    /* eu nao consegui tirar esse break daqui de maneira
		     * eficiente */
		    break;
		}
	    }
	}

	i++;
    }
}

int pos_menor(struct racional v[], int n, int i)
{
    int p;
    if (i == (n - 1))
	return i;

    p = pos_menor(v, n, i + 1);
    return (compara_r(v[i], v[p]) == -1) ? i : p;
}

void selection_sort(struct racional v[], int n, int i)
{
    int p;
    struct racional tmp;
    if (n > i) {
	p = pos_menor(v, n, i);
	tmp = v[i];
	v[i] = v[p];
	v[p] = tmp;
	selection_sort(v, n, i + 1);
    }
}

struct racional soma_vec_r(struct racional v[], int n, int i)
{
    struct racional r;
    if (i == (n - 1))
	return v[i];

    soma_r(v[i], soma_vec_r(v, n, i + 1), &r);
    simplifica_r(&r);
    return r;
}

int main()
{
    int size, i;
    struct racional vec[VEC_SIZE];
    struct racional soma;

    srand(0);
    scanf("%d", &size);

    for (i = 0; i < size; i++)
	vec[i] = sorteia_r(SORT_MAX);

    IMPRIME_VEC_R(vec, size);
    printf("\n");

    elimina_invalido(vec, &size);

    IMPRIME_VEC_R(vec, size);
    printf("\n");

    selection_sort(vec, size, 0);

    IMPRIME_VEC_R(vec, size);
    printf("\n");

    soma = soma_vec_r(vec, size, 0);
    imprime_r(soma);
    printf("\n");

    return 0;
}
