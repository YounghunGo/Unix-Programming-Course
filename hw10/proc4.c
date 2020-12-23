#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid;
    int wstatus;

    printf("Parent: mypid = %d Fork!\n", getpid());

    pid = fork();

    if(pid < 0)
    {
        perror("Fork Failed");
        return 1;
    }
    else if(pid == 0)
    {
        printf("Child: I am your child! pid=%d\n", getpid());

        if(execlp("/usr/bin/cat", "cat", "unix.txt", (char *)NULL) == -1)
        {
            perror("execlp");
            exit(1);
        }

        printf("Child: This message is never shown!!\n");
    }
    else
    {
        printf("Parent: I am your father! pid=%d\n", getpid());
        wait(&wstatus);
        printf("Parent: Child says %d\n", wstatus);
    }
}