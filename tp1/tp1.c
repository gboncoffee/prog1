#include <stdio.h>
#include <stdlib.h>
#include "racionais.h"

int main()
{
	for (int i = 1; i <= 10; i++)
		printf("%d mdc 10 = %d\n", i, mdc(i, 10));

	return 0;
}
