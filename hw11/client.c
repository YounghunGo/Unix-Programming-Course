#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>

#define BUFSIZE (80)

int main(int argc, char* argv[])
{
    key_t key;
    int shmid, size;
    char buf[BUFSIZE];
    void *shmaddr;

    key = ftok("shmfile", 1);   //shmfile을 이용해 key 생성. 파일이 존재해야 함.
    size = sysconf(_SC_PAGESIZE) * 4;   //페이지 사이즈 * 4 = 16KB
    shmid = shmget(key, size, IPC_CREAT|0666);//공유 메모리 접근 권한 설정

    shmaddr = shmat(shmid, NULL, 0); //할당받은 공유 메모리 공간의 주소를 얻음.

    printf("Shared memory info: key=%d shmid=%d shmaddr=%p\n", key, shmid, shmaddr);

    memcpy(buf, shmaddr, BUFSIZE);//해당 공간의 처음 80B만큼의 데이터 복사하여 출력 
    printf("Client: %s\n", buf);

    strcpy(buf, "Hello Server! I am Client.");
    memcpy(shmaddr, buf, BUFSIZE);//buf의 문자열을 공유 메모리 공간으로 복사

    shmdt(shmaddr);//공유 메모리 공간 연결 해제

    strcpy(buf, "Message is deleted");//buf 문자열 수정
    memcpy(shmaddr, buf, BUFSIZE);//buf의 문자열을 다시 기존 공유 메모리 공간으로 복사 시도
                                    //그러나 연결 해제 되었으므로, 할당되지 않은 공간에 접근 -> seg- fault
    return 0;
}
//key와 shmid는 같은데 shmaddr이 다른 이유는 shared memory가 각 프로세스의 메모리