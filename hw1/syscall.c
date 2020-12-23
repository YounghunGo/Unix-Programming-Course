#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>

int main(void)
{
	printf("My pid: %d\n", getpid());
	printf("My pid: %d by syscall()\n", (int) syscall(39));

	sleep(1);

	return 0;
}
