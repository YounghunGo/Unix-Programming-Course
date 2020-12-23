#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	int fd, count;
	char buf[80];

	if(argc != 2)
	{
		printf("< Usage: ./file3 filename >\n");
		return 1;
	}

	fd = open(argv[1], O_RDWR);
	if(fd == -1)
	{
		perror("Creat");
		exit(1);
	}

	printf("%s is opened! fd = %d\n", argv[1], fd);

	while((count = read(fd, buf, 80)) > 0)
	{
		printf("%d: %s\n", count, buf);
	}

	close(fd);

	return 0;
}
