#include <stdio.h>

int main(void)
{
	int n, l;

	while (scanf("%d", &n) && n) {
		scanf("%d", &l);
		if (n * n != l) {
			printf("0\n");
			return 0;
		}
	}

	printf("1\n");

	return 0;
}
