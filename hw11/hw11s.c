#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define SERVERPORT (7788)
#define BUFFSIZE (4096)

int main(void)
{
    int s_sock, c_sock; //소켓의 file descripter
    struct sockaddr_in server_addr, client_addr;
    socklen_t c_addr_size;
    
    FILE *fp;
    char buf[BUFFSIZE];
    char *send_buf;
    int size;

    char* tosend;
    int remaining;
    int result = 0, sent = 0;

    s_sock = socket(AF_INET, SOCK_STREAM, 0);//AF_INET : 인터넷 소켓(네트워크를 이용한 통신), SOCK_STREAM : TCP 사용

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

    listen(s_sock, 1);//client의 연결을 기다림. backlog는 1

    c_addr_size = sizeof(struct sockaddr);
    c_sock = accept(s_sock, (struct sockaddr *)&client_addr, &c_addr_size);//client를 accept

    if(c_sock == -1)
    {
        perror("Can't accept a connection");
        exit(1);
    }

    printf("** Check: s_sock=%d c_sock=%d\n", s_sock, c_sock);
    printf("** Check: server IP=%s client IP addr=%s port=%d\n", inet_ntoa(server_addr.sin_addr), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

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

    //sleep(3);//server가 먼저 종료되는 경우 client가 port를 놔주지 않는 경우가 생겨서 sleep 해줌 

    close(c_sock);
    close(s_sock);
    fclose(fp);
    free(send_buf);

    printf("종료\n");

    return 0;
}