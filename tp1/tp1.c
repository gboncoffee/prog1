#include <stdio.h>
#include <stdlib.h>
#include "racionais.h"

int main()
{
	srand(0);

	imprime_r(cria_r(6, 4)); /* 3/2 */
	printf("\n");
	imprime_r(cria_r(0, 2)); /* 0 */
	printf("\n");
	imprime_r(cria_r(2, 1)); /* 2 */
	printf("\n");
	imprime_r(cria_r(2, 2)); /* 1 */
	printf("\n");
	imprime_r(cria_r(-3, 2)); /* -3/2 */
	printf("\n");
	imprime_r(cria_r(3, -2)); /* -3/2 */
	printf("\n");
	imprime_r(cria_r(-3, -2)); /* 3/2 */
	printf("\n");
	imprime_r(cria_r(3, 0)); /* INVALIDO */
	printf("\n");

	return 0;
}
