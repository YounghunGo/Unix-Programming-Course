#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
//fork를 통한 자식 프로세스 생성
//SIGUSR1, SIGUSR2에 대해 핸들러 등록

#define BUFSIZE (80)

void handler1(int signo)
{
    printf("(부모->자식) 공간이 할당 되었음.\n");
}
void handler2(int signo)
{
    printf("(자식->부모) 수정이 완료 되었음.)\n");
}

int main()
{
    pid_t pid;
    
    key_t key;
    int shmid, size;
    void *shmaddr;
    char buf[BUFSIZE];
    
    struct sigaction act;
    sigset_t set_to_wait;

    FILE *fp;

    if(signal(SIGUSR1, handler1) == SIG_ERR)//signal 등록
    {
        perror("signal");
        return 3;
    }
    if(signal(SIGUSR2, handler2) == SIG_ERR)//signal 등록    
    {
        perror("signal");
        return 3;
    }
    
    pid = fork();//fork 수행

    if(pid < 0)//fork 실패 시
    {
        perror("fork failed\n");
        return 1;
    }
    else if(pid == 0)//자식 프로세스 수행
    {
        sigfillset(&set_to_wait);
        sigdelset(&set_to_wait, SIGUSR1);
        sigprocmask(SIG_BLOCK, &set_to_wait, NULL);//SIGUSR1 제외한 시그널 블록

        printf("(자식) SIGUSR1 대기 중..\n");
        sigsuspend(&set_to_wait);//시그널 대기
        printf("(자식) SIGUSR1 received!\n");

        key = ftok("shmfile", 1);   //shmfile을 이용해 key 생성. 파일이 존재해야 함.
        size = sysconf(_SC_PAGESIZE);   //페이지 사이즈 = 4KB
        shmid = shmget(key, size, IPC_CREAT|0666);//공유 메모리 접근 권한 설정

        shmaddr = shmat(shmid, NULL, 0); //할당받은 공유 메모리 공간의 주소를 얻음.

        if((fp = fopen("shmfile", "r")) == NULL)//shmfile open
        {
            perror("file open failed\n");
            return 1;
        }

        fread(buf, BUFSIZE, 1, fp);//80B를 읽어 buf에 저장함

        memcpy(shmaddr, buf, BUFSIZE);//공유 메모리에 복사
        sleep(6);

        kill(getppid(), SIGUSR2);//parent에게 SIGUSR2을 보내 공간이 할당되었음을 알림
        
        fclose(fp);
    }
    else//부모
    {
        //4KB 공유 메모리 공간을 할당하고 0으로 초기화
        key = ftok("shmfile", 1);   //shmfile을 이용해 key 생성. 파일이 존재해야 함.
        size = sysconf(_SC_PAGESIZE);   //페이지 사이즈 = 4KB
        shmid = shmget(key, size, IPC_CREAT|0666);//공유 메모리 접근 권한 설정

        shmaddr = shmat(shmid, NULL, 0); //할당받은 공유 메모리 공간의 주소를 얻음.

        memset(shmaddr, 0, size);// 공유 메모리 공간을 0으로 초기화
        
        sleep(3);

        kill(pid, SIGUSR1);//child에게 SIGUSR1을 보내 공간이 할당되었음을 알림

        sigfillset(&set_to_wait);//SIGUSR2를 제외한 시그널을 1로 masking
        sigdelset(&set_to_wait, SIGUSR2);

        sigprocmask(SIG_BLOCK, &set_to_wait, NULL);//SIGUSR2를 제외한 시그널 블록

        sleep(3);

        printf("(부모) SIGUSR2 대기 중..\n");
        sigsuspend(&set_to_wait);//시그널 대기
        printf("(부모) SIGUSR2 received!\n");
        memcpy(buf, shmaddr, BUFSIZE);//해당 공간의 처음 80B만큼의 데이터 복사하여 출력

        printf("(부모) 출력 : %s\n", buf); 
    }
}