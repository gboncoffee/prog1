#include <stdlib.h>
#include <stdio.h>
#include "racionais.h"

#define MODULO(a) (a >= 0 ? a : (-a))

/* mdc pelo Metodo de Euclides recursivo, bem elegante. */
int mdc(int a, int b)
{
    if (!b)
	return a;
    return mdc(b, a % b);
}

/* mmc usando o proprio mdc. */
int mmc(int a, int b)
{
    return MODULO(a * b) / mdc(a, b);
}

/* A funcao seta a flag de validez com base no denominador, entao chamadores nao
 * precisam se preocupar com isso */
struct racional *cria_r(long int num, long int den)
{
    struct racional *r = (struct racional*) malloc(sizeof(struct racional));
    if (r == NULL)
        return NULL;
    /* Um racional eh valido caso o denominador seja diferente de zero,
     * entao basta usar o proprio denominador como flag. */
    r->num = num;
    r->den = den;
    return r;
}

void destroi_r(struct racional *r)
{
    if (r != NULL)
        free(r);
}

long int numerador_r(struct racional *r)
{
    return (r == NULL) ? 0 : r->num;
}

long int denominador_r(struct racional *r)
{
    return (r == NULL) ? 0 : r->den;
}

/* Wrapper para a flag de validez. */
int valido_r(struct racional *r)
{
    return (r == NULL) ? 0 : (int) !r->den;
}

/* Simplifica um racional dividindo ambos os termos pelo mdc deles. */
int simplifica_r(struct racional *r)
{
    if (r == NULL)
        return 0;
    /* Caso mdc seja 0 (ocasiao em que ambos os termos de r sao), usa 1 para
     * nao dar problema em runtime */
    int m = mdc(r->num, r->den);
    m = !m ? 1 : m;
    r->num /= m;
    r->den /= m;
    /* Essa condicao resolve tanto os casos de (-x/-y) e (x/-y) */
    if (r->den < 0) {
        r->num *= (-1);
        r->den *= (-1);
    }

    /* Garante que o retorno verdadeiro é exatamente 1 */
    return (r->den ? 1 : 0);
}

/* Veja as regras de impressao no header. */
void imprime_r(struct racional *r)
{
    if (r == NULL) {
        printf("NULO");
        return;
    }

    simplifica_r(r);
    if (!valido_r(r)) {
        printf("INVALIDO");
        /* Esse else if cobre os seguintes casos: (x/x), (0/x) e (x/1). Em todos
         * nos so queremos imprimir o numerador, ja que o numero ja esta
         * simplificado */
    } else if (!r->num || r->den == 1) {
        printf("%ld", r->num);
    } else {
        if (r->num * r->den < 0)
            printf("-");
        printf("%ld/%ld", MODULO(r->num), MODULO(r->den));
    }
}

int compara_r(struct racional *r1, struct racional *r2)
{
    if (!(valido_r(r1) && valido_r(r2)))
        return 0;

    int m = mmc(r1->den, r2->den);
    int d1 = r1->num * (m / r1->den);
    int d2 = r2->num * (m / r2->den);

    int ret = 0;
    if (d1 < d2)
        ret = -1;
    else if (d1 > d2)
        ret = 1;
    return ret;
}

/* As operacoes sao bem simples, e nao simplificam os numeros. Talvez elas
 * pudessem ser implementadas como macros, mas isso esta fora do escopo e dos
 * requisitos do trabalho. */

/* tp2: Todas as operacoes sao extremamente parecidas em codigo mas nao vi como
   seria possivel otimizar isso. */

int soma_r(struct racional *r1, struct racional *r2, struct racional *r3)
{
    if (!(valido_r(r1) && valido_r(r2) && r3 != NULL))
        return 0;

    r3->den = mmc(r1->den, r2->den);
    r3->num = ((r3->den / r1->den) * r1->num) + ((r3->den / r2->den) * r2->num);

    simplifica_r(r3);

    return 1;
}

int subtrai_r(struct racional *r1, struct racional *r2, struct racional *r3)
{
    if (!(valido_r(r1) && valido_r(r2) && r3 != NULL))
        return 0;

    r3->den = mmc(r1->den, r2->den);
    r3->num = ((r3->den / r1->den) * r1->num) - ((r3->den / r2->den) * r2->num);

    simplifica_r(r3);

    return 1;
}

int multiplica_r(struct racional *r1, struct racional *r2, struct racional *r3)
{
    if (!(valido_r(r1) && valido_r(r2) && r3 != NULL))
        return 0;

    r3->den = r1->den * r2->den;
    r3->num = r1->num * r2->num;

    simplifica_r(r3);

    return 1;
}

int divide_r(struct racional *r1, struct racional *r2, struct racional *r3)
{
    if (!(valido_r(r1) && valido_r(r2) && r3 != NULL))
        return 0;

    r3->den = r1->den * r2->num;
    r3->num = r1->num * r2->den;

    return 1;
}
