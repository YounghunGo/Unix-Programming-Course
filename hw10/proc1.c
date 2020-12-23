#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;

    printf("Parent: mypid = %d Fork!\n", getpid());

    pid = fork();

    if(pid < 0)
    {
        perror("Fork Failed");
        return 1;
    }
    else if(pid == 0)
    {
        sleep(1);
        printf("Child: I am your child! pid=%d\n", getpid());
        return 0;
    }
    else
    {
        sleep(1);
        printf("Parent: I am your father! pid=%d\n", getpid());
        return 0;
    }
}