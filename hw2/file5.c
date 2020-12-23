#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char buf[80];
int fd, count;

void read_five_bytes(void)
{
	if ((count = read(fd, buf, 5)) <= 0)
	{
		perror("Read Error");
		exit(1);
	}
}
int main(int argc, char* argv[])
{

	if(argc != 2)
	{
		printf("< Usage: ./file5 filename >\n");
		return 1;
	}

	fd = open(argv[1], O_RDWR);
	if(fd == -1)
	{
		perror("Open");
		exit(1);
	}

	printf("%s is opened! fd = %d\n", argv[1], fd);

	read_five_bytes();
	printf("\n%d: %s\n", count, buf);
	printf("Currunt position: %ld\n", lseek(fd, 0, SEEK_CUR));

	lseek(fd, 1, SEEK_SET);
	read_five_bytes();
	printf("\n%d: %s\n", count, buf);
	printf("Currunt position: %ld\n", lseek(fd, 0, SEEK_CUR));

        lseek(fd, 2, SEEK_SET);
        read_five_bytes();
        printf("\n%d: %s\n", count, buf);
        printf("Currunt position: %ld\n", lseek(fd, 0, SEEK_CUR));
	

	close(fd);

	return 0;
}
