#include <stdio.h>

int main(void)
{
	int n, m, s;
	scanf("%d", &n);
	m = n * 37;
	s = 0;
	while (m != 0) {
		s += m % 10;
		m /= 10;
	}

	if (s == n)
		printf("SIM\n");
	else
		printf("NAO\n");

	return 0;
}
