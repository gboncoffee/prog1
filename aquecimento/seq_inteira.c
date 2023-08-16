#include <stdio.h>

int main(void)
{
	int n, l, i, s;
	scanf("%d", &n);

	s = n;
	i = 1;
	do {
		l = n;
		scanf("%d", &n);
		i++;
		s += n;
	} while ((n != l * 2) && (n != l / 2));

	printf("%d %d %d %d\n", i, s, l, n);

	return 0;
}
