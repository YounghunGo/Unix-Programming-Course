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
    sigset_t set_to_block;

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);
    act.sa_flags = 0;
    act.sa_handler = handler;

    sigaction(SIGINT, &act, NULL);
    
    sigemptyset(&set_to_block);
    sigaddset(&set_to_block, SIGINT);
    sigprocmask(SIG_BLOCK, &set_to_block, NULL);

    for(i=0; i<10; i++)
    {
        printf("** Waiting for signal %d : %02d seconds\n", SIGINT, i);
        
        if(i==5)
        {
            sigprocmask(SIG_UNBLOCK, &set_to_block, NULL);
        }
        
        sleep(1);
    }
    printf("Finished\n");

    return 0;
}