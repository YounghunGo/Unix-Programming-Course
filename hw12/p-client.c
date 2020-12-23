
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
    int c_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t c_addr_size;
    char buf[BUFFSIZE] = {0};
    char hello[] = "Hello~I am Client!\n";

    c_sock = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));//memset

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//loop-back

    printf("[C] Connecting...\n");

    if(connect(c_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)//connect 시도
    {
        perror("[C] Can't connect to a server");
        exit(1);
    }

    printf("[C] Connected!\n");//connect 성공!

    //1. recv msg from server (maybe it's "hello")
    if(recv(c_sock, buf, BUFFSIZE, 0) == -1)
    {
        perror("[C] Can't receive message");
        exit(1);
    }

    printf("[C] Server says: %s\n", buf);

    //2. say hi to server
    if(send(c_sock, hello, sizeof(hello)+1, 0) == -1)
    {
        perror("[C] Can't send message");
        exit(1);
    }

    printf("[C] I said Hi to Server!!\n");

    //printf("[C] I am going to sleep...\n");
    //sleep(10);

    close(c_sock);

    return 0;
}