#include <stdlib.h>
#include "racionais.h"

/*
 * TODO:
 * [X] aleat (ja testada);
 * [-] mdc; (metodo de euclides recursivo) (ver livro de pascal do castilho)
 * [ ] mmc;
 * [ ] simplifica_r;
 * [ ] cria_r;
 * [ ] sorteia_r;
 * [ ] imprime_r;
 * [ ] valido_r;
 * [ ] soma_r;
 * [ ] subtrai_r;
 * [ ] multiplica_r;
 * [ ] divide_r;
 */

int aleat(int min, int max)
{
	// random() retorna long por isso precisa converter. O modulo colapsa a
	// funcao no intervalo aberto, depois soma com minimo para ficar acima
	// do necessario
	return (int) (random() % ((max - min) + 1)) + min;
}

int mdc(int a, int b)
{
	// Esse for necessariamente para em 1, pois para todo inteiro i,
	// i % 1 = 0
	int comeco = a < b ? a : b;
	for (int i = comeco;; i--)
		if ((a % i == 0) && (b % i == 0))
			return i;
}
