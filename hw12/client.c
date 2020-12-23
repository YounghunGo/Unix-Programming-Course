#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFSIZE (4096)
#define SERVERPORT (7799)

int main(int argc, char* argv[])
{
    //socket 변수
    int c_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t c_addr_size;
    char *buf;
    
    int value = 0;
    char* recv_buffer = (char*)&value;
    int remaining = sizeof(int);
    int received = 0;
    int result = 0;
    int size;

    FILE *fp;

    if(argc != 5)//index field를 추가
    {
        printf("< usage ./executefile ip_address_to_connect port filename_to_request index\n");
        exit(1);
    }

    c_sock = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));//memset

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));//HBO to NBO
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(c_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)//server와 연결 시도
    {
        perror("Can't connect to a server");
        exit(1);
    }

    //send filename
    if(send(c_sock, argv[3], sizeof(argv[3]), 0) == -1)
    {
        perror("Can't send message");
        exit(1);
    }

    while(remaining > 0)
    {
        result = recv(c_sock, recv_buffer + received, remaining, 0);//server에서 전송한 file size를 받아 recv_buffer에 저장

        if(result > 0)
        {
            remaining -= result;
            received += result;
        }
        else if(result < 0)
        {
            perror("errors");
            exit(1);
        }
    }

    size = *(int*)recv_buffer;//기존에 server에서 보내려고 한 int형으로 형변환
    
    buf = calloc(size, sizeof(char)); //size에 맞추어 buf 할당

    printf("size: %d\n", size);

    if(recv(c_sock, buf, size, 0) == -1)//파일의 내용을 받아 buf에 저장
    {
        perror("Can't receive message");
        exit(1);    
    }
    strncat(argv[3], argv[4], 2);
    fp = fopen(argv[3], "w");//파일을 열어
    
    printf("write file : argv[3] : %s\n", argv[3]);

    fwrite(buf, size, 1, fp);//파일에 size만큼 작성

    fclose(fp);
    close(c_sock);
    free(buf);

    return 0;
}