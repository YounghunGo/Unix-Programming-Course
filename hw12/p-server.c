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

    s_sock = socket(AF_INET, SOCK_STREAM, 0);

    int option = 1;
    setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));//port 오류 해결 코드

    bzero(&server_addr, sizeof(server_addr));//memset

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    //server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//loop-back으로 수정

    if(bind(s_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)//bind
    {
        perror("Can't bind a socket");
        exit(1);
    }

    listen(s_sock, 1);//backlog == 1

    c_addr_size = sizeof(struct sockaddr);//client 정보 저장할 구조체 size 초기화 해줘야 함. 안 할 시 밑에서 오류?
    
    for(i=0; i<3; i++)//fork 3번 수행
    {
        printf("[S] waiting for a client..#%02d\n", i);

        if((c_sock = accept(s_sock, (struct sockaddr *) &client_addr, &c_addr_size)) == -1)//accept
        {
            perror("error");//accept 실패 시 error
            exit(1);
        }
        else//accept 성공 시
        {
            printf("[S] Connected: client IP addr=%s port=%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            pid = fork();//fork 수행

            if(pid < 0)
            {
                perror("fork failed");
                exit(1);
            }
            else if(pid == 0)//child
            {
                //1. say hello to clinet
                printf("[SS] Service: I am your child! pid=%d\n", getpid());

                if(send(c_sock, hello, sizeof(hello)+1, 0) == -1)
                {
                    perror("[SS] Can't send message");
                    exit(1);
                }

                printf("[SS] I said Hello to Client!\n");

                //2. recv msg from client
                if(recv(c_sock, buf, BUFFSIZE, 0) == -1)
                {
                    perror("[SS] Can't receive message");
                    exit(1);
                }

                printf("[SS] Client says: %s\n", buf);

                close(c_sock);

                exit(0);
            }
            continue;//parent는 continue
        }
    }
    //sleep(60);
    for(i=0; i<3; i++)
    {
        int tpid = wait(&status);

        printf("[S] Child #%02d is finished with %d\n", i, WEXITSTATUS(status));
    }

    close(s_sock);
    close(c_sock);
    
    return 0;
}