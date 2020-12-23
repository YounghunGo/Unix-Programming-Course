#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	FILE *rfp, *wfp;
	int i=0, count;
	char buf[80];

	if(argc != 3)
	{
		printf("< Usage: ./file3 file_for_read file_for_write >\n");
		return 1;
	}

	rfp = fopen(argv[1], "r");
	if(rfp == NULL)
	{
		perror("Open file for read");
		exit(1);
	}

	wfp = fopen(argv[2], "w");
	if(wfp == NULL)
	{
		perror("Open file for write");
		exit(1);
	}

	printf("%s and %s are opened! rfp = %p wfp = %p \n", argv[1], argv[2], rfp, wfp);

	while((count = fread(buf, 1, 10, rfp)) > 0)
	{
		printf("%d: count=%d\n", i++, count);
		fwrite(buf, 1, count, wfp);
	}

	fclose(rfp);
	fclose(wfp);
}
