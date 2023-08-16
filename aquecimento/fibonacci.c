#include<stdio.h>

int main(void)
{
	int n, tmp;
	int a = 1;
	int b = 1;
	scanf("%d", &n);

	do {
		printf("%d, ", a);
		tmp = a;
		a = b;
		b += tmp;

		n--;
	} while (n);

	printf("\n");

	return 0;
}
