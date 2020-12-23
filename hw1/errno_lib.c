#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

extern int errno;

int main(void)
{
	FILE *fp;

	if ((fp = fopen("unix.txt", "r")) == NULL)
	{
		printf("errno=%d\n", errno);
		return 1;
	}
	printf("파일이 정상적으로 열렸습니다.\n");
	fclose(fp);

	return 0;
}
