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

    memset(shmaddr, 0, size);// 공유 메모리 공간 0으로 초기화

    memcpy(buf, shmaddr, BUFSIZE);//해당 공간의 처음 80B만큼의 데이터 복사하여 출력 
    printf("Initialized: %s\n", buf);

    printf("Waiting for client...\n");

    sleep(3);

    memcpy(buf, shmaddr, BUFSIZE);
    printf("Changed: %s\n", buf);

    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}