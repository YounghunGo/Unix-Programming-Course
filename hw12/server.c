#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define SERVERPORT (7799)
#define BUFFSIZE (4096)
#define MAX_THREADS (10)

pthread_t tid[3];//data 영역에 배치되어 다른 thread들도 접근 할 수 있음
int s_sock, *status, *tcounts;//server socket 변수와 heap에서 사용할 변수들
char *send_buf;
int cnt;//총 생성된 thread 갯수 counting
int args[MAX_THREADS];//8192kb 범인
FILE *fp;
int *ret;//thread return 변수

void* transfer(void *arg)
{
    int c_sock = *((int *)arg);//인자로 넘어온 c_sock 번호
    //파일 transfer에 사용할 변수
    char* tosend;
    int remaining;
    int result = 0, sent = 0;
    char buf[BUFFSIZE];
    int size;
    FILE *fp;

    if(recv(c_sock, buf, BUFFSIZE, 0) == -1)//file name을 receive
    {
        perror("Can't receive message");
        exit(1);
    }

    printf("thread says : filename : %s\n", buf);
    //sleep(60);
    
    if(fp == NULL)//실험에서는 하나의 포인터만 사용해서 한번만 open
    {
        printf("file opening\n");
        fp = fopen(buf, "r");
    }
    //sleep(60);
    fseek(fp, 0L, SEEK_END); //file의 크기를 구함
    size =  ftell(fp);
    rewind(fp);//초기 설정으로 되돌림

    printf("thread says : filesize : %d\n", size);
    
    tosend = (char*)&size;//파일 size를 client에게 전송
    remaining = sizeof(size);//int형 데이터를 socket에 전송하기 위해선 형변환이 필요
    result = 0, sent = 0;
    

    while(remaining > 0)
    {
        result = send(c_sock, tosend+sent, sizeof(int), 0);//file size를 client에 send
        if(result > 0)
        {
            remaining -= result;
            sent += remaining;
        }
        else if(result < 0)
        {
            perror("error");
            exit(1);
        }
    }
    //전송 완료
    
    if(send_buf == NULL)//send_buf도 한번만 할당
    {
        printf("send buffer creating\n");
        send_buf = calloc(size, sizeof(char));
    }
        //send 버퍼 생성

    if(fread(send_buf, size, 1, fp) > 0)//파일의 size만큼 읽어 send_buf에 저장
    {
        if(send(c_sock, send_buf, size, 0) < 0)//socket에 실어 send
        {
            perror("failed to send file\n");
            exit(1);
        }
    }

    printf("thread says : file transfer succeed!\n");
    
    (*tcounts)--;//critical section. 동기화 문제가 생길 수 있다.
    close(c_sock);
    fclose(fp);
    pthread_exit(ret);
}

void handler(int signo)//ctrl + c 수행 시
{
    psignal(signo, "Received signal:");

    printf("exit on\n");

    for(int i=0; i<cnt; i++)//현재까지 생성된 thread의 종료를 확인한다.
    {
        pthread_join(tid[i], (void **)&status);
        printf("Thread no.%d ends: %d\n", i, *status);
    }

    //fclose(fp);
    free(ret);
    free(send_buf);
    free(tcounts);
    close(s_sock);

    //sleep(10);
}

int main(void)
{
    //핸들러 정의
    struct sigaction act;

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);
    act.sa_flags = 0;
    act.sa_handler = handler;

    if(sigaction(SIGINT, &act, (struct sigaction *)NULL) < 0)
    {
        perror("sigaction");
        return 3;
    }

    //소켓 사용을 위한 변수
    struct sockaddr_in server_addr, client_addr;
    socklen_t c_addr_size;
    
    int c_sock;

    ret = (int *)malloc(sizeof(int));//thread return value 힙 메모리로 사용
    *ret = 0;

    tcounts = calloc(1, sizeof(int));//동시 동작 thread 사용을 위한 tcounts

    s_sock = socket(AF_INET, SOCK_STREAM, 0);//AF_INET : 인터넷 소켓(네트워크를 이용한 통신), SOCK_STREAM : TCP 사용

    int option = 1;
    setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    bzero(&server_addr, sizeof(server_addr));//server_addr 초기화

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);//HBO to NBO for 16-bit address
    //server_addr.sin_addr.s_addr = inet_addr(INADDR_ANY);//동작 중인 호스트 시스템의 IP주소를 자동으로 받아온다?
    server_addr.sin_addr.s_addr = INADDR_ANY;//동작 중인 호스트 시스템의 IP주소를 자동으로 받아온다

    printf("server IP : %s\n", inet_ntoa(server_addr.sin_addr));

    if(bind(s_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)//소켓 이름 지정
    {
        perror("Can't bind a socket");
        exit(1);
    }

    listen(s_sock, 10);//client의 연결을 기다림. backlog는 10

    c_addr_size = sizeof(struct sockaddr);
    
    int idx;

    while(1)//SIGINT 시그널이 분기를 종료할 것임
    {
        while((*tcounts) == 10)//busy waiting 방식. 
        {                       //semaphore를 사용하면 cpu 자원을 효율적으로 사용할 수 있지만 이 과제에서는 busy waiting 방식 사용.
            //no operation
        }

        printf("currunt thread : %d\n", *tcounts);//동시 동작 thread 출력
        printf("total count of threads : %d\n\n", cnt);//현재까지 생성된 thread 갯수 출력

        if((c_sock = accept(s_sock, (struct sockaddr *)&client_addr, &c_addr_size)) != -1)//accept 성공 시
        {
            for(int i=0; i<10; i++)//빈 index를 찾는다.
            {
                if(args[i] == 0)
                {
                    idx = i;        
                }
            }
            args[idx] = c_sock;//빈 index에 c_sock 할당
            
            int check = pthread_create(&tid[cnt++], NULL, transfer, &args[idx]);//thread 생성, 인자로 c_sock을 넘김

            if(check != 0)
            {
                perror("failed to create thread");
                exit(1);
            }

            (*tcounts)++;//critical section. 동기화 문제가 발생할 수 있음.
        }
        else
        {
            perror("Can't accept a connection, do exit process");
            return 1;
        }
    }

    return 0;
}