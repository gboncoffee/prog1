#include <stdlib.h>
#include <stdio.h>
#include "racionais.h"

#define MODULO(a) (a >= 0 ? a : (-a))

/*
 * TODO:
 * [X] aleat (ja testada);
 * [X] mdc; (metodo de euclides recursivo) (ver livro de pascal do castilho)
 * [X] mmc;
 * [X] simplifica_r;
 * [X] cria_r;
 * [X] sorteia_r;
 * [X] imprime_r;
 * [X] valido_r;
 * [ ] soma_r;
 * [ ] subtrai_r;
 * [ ] multiplica_r;
 * [ ] divide_r;
 */

/* aleat NAO inicializa srand(). */
int aleat(int min, int max)
{
	/* O modulo colapsa a funcao no intervalo aberto, depois soma com minimo
	 * para ficar acima do necessario. */
	return (rand() % ((max - min) + 1)) + min;
}

/* mdc pelo Metodo de Euclides recursivo, bem elegante. */
int mdc(int a, int b)
{
	if (!b)
		return a;
	else
		return mdc(b, a % b);
}

/* mmc usando o proprio mdc. */
int mmc(int a, int b)
{
	return MODULO(a * b) / mdc(a, b);
}

/* Cria novo racional (seta a flag de valido com base no denominador). */
struct racional cria_r(int num, int den)
{
	struct racional r;
	/* Um racional eh valido caso o denominador seja diferente de zero,
	 * entao basta usar o proprio denominador convertido para unsigned
	 * short. */
	r.valido = (unsigned short) den;
	r.num = num;
	r.den = den;
	return r;
}

/* Wrapper para a flag de validez. */
int valido_r(struct racional r)
{
	return r.valido;
}

/* Simplifica um racional dividindo ambos os termos pelo mdc deles. */
struct racional simplifica_r(struct racional r)
{
	/* Caso mdc seja 0 (ocasiao em que ambos os termos de r sao), usa 1 para
	 * nao dar problema em runtime */
	int m = mdc(r.num, r.den);
	m = !m ? 1 : m;
	return cria_r(r.num / m, r.den / m);
}

/* Retorna um racional aleatorio simplificado. Pode ser invalido. */
struct racional sorteia_r(int n)
{
	int num = aleat(0, n);
	int den = aleat(0, n);
	return simplifica_r(cria_r(num, den));
}

void imprime_r(struct racional r)
{
	r = simplifica_r(r);
	if (!valido_r(r)) {
		printf("INVALIDO");
		/* Isso ja cobre o caso de x/x pois esta simplificado */
	} else if (!r.num || r.den == 1) {
		printf("%d", r.num);
	} else {
		if (r.num * r.den < 0)
			printf("-");
		printf("%d/%d", MODULO(r.num), MODULO(r.den));
	}
}
