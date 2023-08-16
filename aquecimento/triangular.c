#include <stdio.h>
#include <math.h>

int main(void)
{
	int n;
	int i;
	scanf("%d", &n);

	for (i = 1; i < sqrt(n); i++) {
		if ((i * (i + 1) * (i + 2)) == n) {
			printf("1\n");
			return 0;
		}
	}

	printf("0\n");

	return 0;
}
