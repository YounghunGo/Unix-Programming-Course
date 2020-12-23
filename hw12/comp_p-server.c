#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFSIZE (4096)
#define SERVERPORT (7799)

int main(void)
{
    int s_sock, c_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t c_addr_size;
    char buf[BUFFSIZE] = {0};
    char hello[] = "Hello~I am Server!\n";
    int i=0, status;
    pid_t pid;

    FILE *fp;
    //char buf[BUFFSIZE];
    char *send_buf;
    int size;

    char* tosend;
    int remaining;
    int result = 0, sent = 0;

    s_sock = socket(AF_INET, SOCK_STREAM, 0);

    int option = 1;
    setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = inet_addr("10.0.0.32");

    if(bind(s_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Can't bind a socket");
        exit(1);
    }

    listen(s_sock, 1);//backlog

    c_addr_size = sizeof(struct sockaddr);//client 정보 저장할 구조체 size 초기화 해줘야 함. 안 할 시 밑에서 오류?
    
    for(i=0; i<3; i++)
    {
        printf("[S] waiting for a client..#%02d\n", i);

        if((c_sock = accept(s_sock, (struct sockaddr *) &client_addr, &c_addr_size)) == -1)
        {
            perror("error");
            exit(1);
        }
        else
        {
            printf("[S] Connected: client IP addr=%s port=%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            pid = fork();

            if(pid < 0)
            {
                perror("fork failed");
                exit(1);
            }
            else if(pid == 0)
            {
                //1. 파일 전송
                //파일의 이름과 크기를 받는다
                //파일의 크기를 받아서 위의 size로 버퍼 동적할당

                //파일의 이름을 받아 buf에 저장
                if(recv(c_sock, buf, BUFFSIZE, 0) == -1)
                {
                    perror("Can't receive message");
                    exit(1);
                }

                printf("filename : %s\n", buf);

                if((fp = fopen(buf, "r")) == NULL)//파일이 존재하지 않을 경우 종료
                {
                    perror("file is not exist");
                    exit(1);
                }
                
                sleep(60);
                
                //file의 크기를 구함
                fseek(fp, 0L, SEEK_END);
                size =  ftell(fp);
                rewind(fp);//초기 설정으로 되돌림

                printf("filesize : %d\n", size);
                
                //파일 size를 client에게 전송
                tosend = (char*)&size;//int형 데이터를 socket에 전송하기 위해선 형변환이 필요
                remaining = sizeof(size);
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

                send_buf = calloc(size, sizeof(char));//send 버퍼 생성

                if(fread(send_buf, size, 1, fp) > 0)//파일의 size만큼 읽어 send_buf에 저장
                {
                    if(send(c_sock, send_buf, size, 0) < 0)//socket에 실어 send
                    {
                        perror("failed to send file\n");
                        exit(1);
                    }
                }

                sleep(60);
                exit(0);
            }
            continue;
        }
    }

    sleep(60);

    close(s_sock);
    close(c_sock);
    free(send_buf);
    fclose(fp);

    for(i=0; i<3; i++)
    {
        int tpid = wait(&status);

        printf("[S] Child #%02d is finished with %d\n", i, WEXITSTATUS(status));
    }

    
    
    return 0;
}