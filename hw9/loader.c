#include <stdio.h>
#include <stdlib.h>

struct pscore
{
    int num;
    char nme[10];
    float sc1;
    float sc2;
    float sum;
    struct pscore *next;//자기 참조
};

typedef struct pscore score;

int main(int argc, char* argv[])
{
    score *head, *cur, *prev;//현재 가리키는 포인터, 현재 포인터, 이전 포인터
    FILE *fp;
    int n, cnt = 0;

    printf("1000 이하의 양의 정수를 입력하시오 : ");
    scanf("%d", &n);

    while(n <= 0 || n > 1000)
    {
        printf("1000 이하의 양의 정수를 다시 입력하시오\n");
        scanf("%d", &n);        
    }

    if(argc != 2)
    {
        printf("< Usage ./%s filename_for_read >\n", __FILE__);
        return 1;
    }

    if((fp = fopen(argv[1] , "rb")) == NULL)//read binary
    {
        perror("Open");
        exit(1);
    }

    cur = (score*) malloc(sizeof(score));//우선적으로 힙 공간 할당
    head = cur; 
    prev = cur;
    cur->next = NULL;

    while(fread(cur, 28, 1, fp))//28byte씩 file로부터 읽어옴
    {
        cnt++;

        if(n == cnt)
        {
            fprintf(stdout, "%d %s %.2f %.2f %.2f\n", cur->num, cur->nme, cur->sc1, cur->sc2, cur->sum);
        }

        cur->next = (score*) malloc(sizeof(score));//새로운 데이터가 들어갈 공간 할당
        prev = cur;//cur, prev 이동
        cur = cur->next;
        cur->next = NULL;
    }

    free(cur); //마지막에 할당된 공간은 불필요하므로 해제
    prev->next = NULL;
    
    if(head == cur) //아무것도 할당받지 못한 상태
    {
        head == NULL;
    }
    cur = head;
    
    while(cur != NULL)//head부터 free
    {
        //fprintf(stdout, "free : %d %s %.2f %.2f %.2f\n", cur->num, cur->nme, cur->sc1, cur->sc2, cur->sum);

        prev = cur;
        cur = cur->next;
        free(prev);
    }

	fclose(fp);

    return 0;
}