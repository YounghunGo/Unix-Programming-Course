#include <stdio.h>

int main()
{
	double arr[5];
	double sum = 0;

	arr[0] = 98.56;
	arr[1] = 78.62;
	arr[2] = 78.69;
	arr[3] = 89.32;
	arr[4] = 95.29;

	for(int i=0; i<5; i++)
	{
		sum += arr[i];	
	}

	printf("%.2f\n%.2f\n", sum, sum/(5.0));
}
