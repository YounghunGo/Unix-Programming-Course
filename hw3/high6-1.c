#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_BUF (80)

int main(int argc, char* argv[])
{
	FILE* fp;
	int line = 0;
	char buf[MAX_BUF];
	

	if(argc != 2)
	{
		printf("< Usage: ./high6 filename >\n");
		return 1;
	}

	if((fp = fopen(argv[1], "r")) == NULL)
	{
		perror("Open");
		exit(1);
	}

	while(!feof(fp))
	{
		fgets(buf, MAX_BUF, fp);
		fprintf(stdout, "%3d: %s", line++, buf);
	}

	fclose(fp);

	return 0;
}
