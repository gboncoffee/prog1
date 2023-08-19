#include <stdlib.h>
#include <stdio.h>
#include "racionais.h"

#define MODULO(a) (a >= 0 ? a : (-a))

/* aleat NAO inicializa srand(). O chamador deve ter inicializado!!! */
int aleat(int min, int max)
{
	/* O modulo e a soma colapsam a funcao no intervalo fechado. */
	return (rand() % ((max - min) + 1)) + min;
}

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
struct racional cria_r(int num, int den)
{
	struct racional r;
	/* Um racional eh valido caso o denominador seja diferente de zero,
	 * entao basta usar o proprio denominador como flag. */
	r.valido = (unsigned short) den;
	r.num = num;
	r.den = den;
	return r;
}

/* Wrapper para a flag de validez. */
int valido_r(struct racional r)
{
	return (int) r.valido;
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

/* Retorna um racional aleatorio simplificado. Pode ser invalido. Usa aleat,
 * entao o chamador DEVE ter inicializado srand() */
struct racional sorteia_r(int n)
{
	return simplifica_r(cria_r(aleat(0, n), aleat(0, n)));
}

/* Veja as regras de impressao no header. */
void imprime_r(struct racional r)
{
	r = simplifica_r(r);
	if (!valido_r(r)) {
		printf("INVALIDO");
	/* Esse else if cobre os seguintes casos: (x/x), (0/x) e (x/1). Em todos
	 * nos so queremos imprimir o numerador, ja que o numero ja esta
	 * simplificado */
	} else if (!r.num || r.den == 1) {
		printf("%d", r.num);
	} else {
		if (r.num * r.den < 0)
			printf("-");
		printf("%d/%d", MODULO(r.num), MODULO(r.den));
	}
}

/* As operacoes sao bem simples, e nao simplificam os numeros. Talvez elas
 * pudessem ser implementadas como macros, mas isso esta fora do escopo e dos
 * requisitos do trabalho. */

struct racional soma_r(struct racional r1, struct racional r2)
{
	int den = mmc(r1.den, r2.den);
	int num = ((den / r1.den) * r1.num) + ((den / r2.den) * r2.num);
	return cria_r(num, den);
}

struct racional subtrai_r(struct racional r1, struct racional r2)
{
	int den = mmc(r1.den, r2.den);
	int num = ((den / r1.den) * r1.num) - ((den / r2.den) * r2.num);
	return cria_r(num, den);
}

struct racional multiplica_r(struct racional r1, struct racional r2)
{
	return cria_r(r1.num * r2.num, r1.den * r2.den);
}

struct racional divide_r(struct racional r1, struct racional r2)
{
	/* Poderia fazer uma chamada para multiplica_r com o inverso porém
	 * julguei que seria desnecessario, dada simplicidade da operacao. */
	return cria_r(r1.num * r2.den, r1.den * r2.num);
}
