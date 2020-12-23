#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFSIZE (4096)
#define SERVERPORT (7799)

int main(void)
{
    int s_sock, c_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t c_addr_size;
    char buf[BUFFSIZE] = {0};
    char hello[] = "Hello~I am Server!\n";
    
    s_sock = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    //server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_addr.s_addr = inet_addr("10.0.0.32");

    if(bind(s_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Can't bind a socket");
        exit(1);
    }

    listen(s_sock, 1);//backlog

    c_addr_size = sizeof(struct sockaddr);//client 정보 저장할 구조체 size 초기화 해줘야 함. 안 할 시 밑에서 오류?
    c_sock = accept(s_sock, (struct sockaddr *) &client_addr, &c_addr_size);//sockaddr_in을 sockaddr로 캐스팅, 안해도 될 것 같다는데??

    if(c_sock == -1)//accept 실패 시
    {
        perror("Can't accept a connection");
        exit(1);
    }

    printf("** Check: s_sock=%d c_sock=%ds\n", s_sock, c_sock);
    printf("** Check: client IP addr=%s port=%ds\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    //1. say hello to clinet
    if(send(c_sock, hello, sizeof(hello)+1, 0) == -1)
    {
        perror("Can't send message");
        exit(1);
    }

    printf("I said Hello to Client!\n");

    //2. recv msg from client
    if(recv(c_sock, buf, BUFFSIZE, 0) == -1)
    {
        perror("Can't receive message");
        exit(1);
    }

    printf("Client says: %s\n", buf);

    close(c_sock);
    close(s_sock);

    return 0;
}