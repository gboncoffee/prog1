#include <stdio.h>
#include <stdlib.h>
#include "racionais.h"

int main()
{
	int n, max, i;
	struct racional r1, r2;

	srand(0);

	scanf("%d", &n);
	scanf("%d", &max);

	for (i = 1; i <= n; i++) {
		printf("%i: ", i);

		r1 = sorteia_r(max);
		r2 = sorteia_r(max);

		imprime_r(r1);
		printf(" ");
		imprime_r(r2);
		printf(" ");

		/* Evita computar as contas caso haja invalidez. */
		if (!valido_r(r1) || !valido_r(r2) || !r2.num) {
			printf("NUMERO INVALIDO\n");
			return 1;
		}

		imprime_r(soma_r(r1, r2));
		printf(" ");
		imprime_r(subtrai_r(r1, r2));
		printf(" ");
		imprime_r(multiplica_r(r1, r2));
		printf(" ");
		imprime_r(divide_r(r1, r2));
		printf("\n");
	}

	return 0;
}
