#include<stdio.h>

int factorial(int x)
{
	if (!x)
		return 1;
	return x * factorial(x - 1);
}

int main(void)
{
	int n;
	scanf("%d", &n);

	printf("%d\n", factorial(n));

	return 0;
}
