#include <stdio.h>
#include <math.h>

int main(void)
{
	int num;

	printf("Please input an integer:");

	scanf("%d", &num);

	printf("%d^2 = %f\n", num, pow(num, 2));

	return 0;
}
