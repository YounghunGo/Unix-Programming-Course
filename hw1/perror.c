#include <unistd.h>
#include <stdio.h>

int main(void)
{
	if(access("unix.txt", F_OK) == -1)
	{
		perror("my message");
		return 1;
	}
	return 0;
}
