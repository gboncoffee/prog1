#include <stdlib.h>
#include <stdio.h>
#include "racionais.h"

#define VEC_SIZE 100

#define IMPRIME_VEC_R(v, s) {			\
		int i;				\
		for (i = 0; i < s; i++)	{	\
			imprime_r(v[i]);	\
			printf(" ");		\
		}				\
	}

int main()
{
	int size, i;
	struct racional vec[VEC_SIZE];

	srand(0);
	scanf("%d", &size);

	for (i = 0; i < size; i++)
		vec[i] = sorteia_r(size);

	IMPRIME_VEC_R(vec, size);

	return 0;
}
