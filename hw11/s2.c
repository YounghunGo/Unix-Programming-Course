#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handler(int signo)
{
    psignal(signo, "Received signal:");
}

int main(int argc, char* argv[])
{
    int i, signo;

    if(argc != 2)
    {
        printf("< Usage: ./s2 signal_to_wait>\n");
        return 1;
    }

    signo = atoi(argv[1]);

    if(signal(signo, handler) == SIG_ERR)
    {
        perror("signal");
        return 3;
    }

    for(i=0; i<10; i++)
    {
        printf("** Waiting for signal %d: %02d seconds\n", signo, i);
        sleep(1);
    }
    
    printf("Finished\n");

    return 0;
}