#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	int fd;
	char name[] = "unix.txt";
	mode_t mode;

	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	fd = open(name, O_CREAT|O_EXCL, mode);
	if(fd == -1)
	{
		perror("Creat");
		exit(1);
	}

	printf("%s is opened! fd = %d\n", name, fd);
	close(fd);

	return 0;
}
