#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREADS (10)

pthread_t tid[MAX_THREADS];//data 영역에 배치되어 다른 thread들도 접근 할 수 있음

void* counting(void *arg)//쓰레드들에게 시킬 일
{
    int i, indent = *((int *)arg), *ret;
    char buf[80];

    srand((unsigned int) indent);
    sleep(rand()%3);//최대 2초 간격으로 sleep

    for(i=0; i<indent; i++)
        buf[i] = '\t';

    for(i=0; i<5; i++)
    {
        printf("%s%d...\n", buf, i);
        sleep(1);
    }

    printf("%sFINISHED!\n", buf);

    ret = (int *)malloc(sizeof(int));
    *ret = indent;
    pthread_exit(ret);
}

int main(int argc, char* argv[])
{
    int i, threads, *status, tcounts = 0, args[MAX_THREADS];

    if(argc != 2)
    {
        printf("Usage: ./thread <a number of thread>\n");
        return 1;
    }

    threads = atoi(argv[1]);

    if(threads >= 1 && threads > MAX_THREADS)
    {
        printf("Max. number of thread is %d! Your input: %d\n", MAX_THREADS, threads);
        return 2;
    }

    printf("Your input: %d\n", threads);

    for(i=0; i<threads; i++, tcounts++)
    {
        args[i] = i;
        if(pthread_create(&tid[i], NULL, counting, &args[i]) != 0)
        {
            perror("Failed to create thread");
            goto exit;
        }
    }

exit:
    for(i=0; i<tcounts; i++)
    {
        pthread_join(tid[i], (void **)&status);
        printf("Thread no.%d ends: %d\n", i, *status);
    }

    return 0;
}
