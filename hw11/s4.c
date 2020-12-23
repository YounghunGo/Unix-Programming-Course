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
    int i;
    struct sigaction act;

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);
    act.sa_flags = 0;
    
    act.sa_handler = SIG_IGN;

    if(sigaction(SIGINT, &act, (struct sigaction *)NULL) < 0)
    {
        perror("sigaction");
        return 3;
    }

    for(i=0; i<10; i++)
    {
        printf("** Waiting for signal %d : %02d seconds\n", SIGINT, i);
        sleep(1);
    }
    printf("Finished\n");

    return 0;
}