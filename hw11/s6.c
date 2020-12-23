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
    sigset_t set_to_wait;

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);
    act.sa_flags = 0;
    act.sa_handler = handler;

    sigaction(SIGINT, &act, NULL);

    sigfillset(&set_to_wait);//mask를 전부 1로 바꿈
    sigdelset(&set_to_wait, SIGINT);//sigint만 0

    printf("** Waiting for signal %d ...\n", SIGINT);

    sigsuspend(&set_to_wait);

    printf("** we got it!\n");

    return 0;
}