#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char buf[80];
FILE* fp;
int count;

void read_five_bytes(void)
{
	if ((count = fread(buf, 1, 5, fp)) <= 0)
	{
		perror("Read Error");
		exit(1);
	}
}
int main(int argc, char* argv[])
{

	if(argc != 2)
	{
		printf("< Usage: ./high2 filename >\n");
		return 1;
	}

	fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
		perror("Open");
		exit(1);
	}

	printf("%s is opened! fd = %p\n", argv[1], fp);

	read_five_bytes();
	printf("\n%d: %s\n", count, buf);
	printf("Currunt position: %ld\n", ftell(fp));

	fseek(fp, 1, SEEK_SET);
	read_five_bytes();
	printf("\n%d: %s\n", count, buf);
	printf("Currunt position: %ld\n", ftell(fp));

        fseek(fp, 2, SEEK_SET);
        read_five_bytes();
        printf("\n%d: %s\n", count, buf);
        printf("Currunt position: %ld\n", ftell(fp));
	

	fclose(fp);

	return 0;
}
