#include "racionais.h"

void inicia_vetor(struct racional **v, int n)
{
    int i, den, num;
    for (i = 0; i < n; i++) {
        scanf("%d", &den);
        scanf("%d", &num);
        v[i] = cria_r(num, den);
    }
}

void imprime_vetor(struct racional **v, int n)
{
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
            while ((*n) - 1 > i) {
                (*n)--;
                if (valido_r(v[*n])) {
                    v[i] = v[*n];
                    /* eu nao consegui tirar esse break daqui de maneira
                     * eficiente */
                    break;
                }
                destroi_r(v[*n]);
            }
        }

        i++;
    }
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
    struct racional* r, n;
    if (i == (n - 1))
        return cria_r(numerador_r(v[i]), denominador_r(v[i]));

    n = cria_r(0, 1);
    r = soma_vec_r(v, n, i + 1);

    soma_r(v[i], r, &n);
    simplifica_r(&r);
    return r;
}

int main ()
{
    /* vetor de ponteiros para racionais */
    struct racional **v;  /* equivalente a struct racional *v[] */

    int n;
    scanf("%d", &n);

    v = (struct racional **) malloc(sizeof(struct racional *));

    inicia_vetor(v, n);
    imprime_r(v, n);
    print("\n");
    elimina_invalido(v, n);
    imprime_r(v, n);
    printf("\n");
    selection_sort(v, n);
    imprime_r(v, n);
    printf("\n");

    

    return 0;
}
