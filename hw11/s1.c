#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int signo;
    pid_t pid;

    if(argc != 3)
    {
        printf("< Usage: ./s1 signo pid>\n");
        return 1;
    }

    signo = atoi(argv[1]);
    pid = (pid_t) atoi(argv[2]);

    printf("Sending signal %d to %d\n", signo, pid);

    kill(pid, signo);

    return 0;
}