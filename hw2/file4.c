#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	int rfd, wfd, count; 
	char buf[80];

	if(argc != 3)
	{
		printf("< Usage: ./file4 file_for_read file_for_write >\n");
		return 1;
	}

	rfd = open(argv[1], O_RDONLY);
	if(rfd == -1)
	{
		perror("Open file for read");
		exit(1);
	}

	wfd = open(argv[2], O_RDWR|O_CREAT|O_EXCL, 0644);
	if(wfd == -1)
	{
		perror("Open file for write");
		exit(1);
	}

	printf("%s and %s are opened! rfd = %d wfd = %d\n", argv[1], argv[2], rfd, wfd);

	while((count = read(rfd, buf, 10)) > 0)
	{
		write(wfd, buf, count);
	}
	
	close(rfd);
	close(wfd);

	return 0;
}
