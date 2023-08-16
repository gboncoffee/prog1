#include<stdio.h>

int eh_perfeito(int n)
{
	int d = 0;
	int i;
	for (i = 1; i < (n / 2) + 1; i++) {
		if (!(n % i))
			d += i;
	}

	return d == n;
}

int main(void)
{
	int k, i;
	scanf("%d", &k);
	i = 6;

	while (k) {
		if (eh_perfeito(i)) {
			k--;
			printf("%d ", i);
		}
		i++;
	}
	putchar('\n');

	return 0;
}
