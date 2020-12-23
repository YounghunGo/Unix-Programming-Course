#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
char* names[10] = { "Alice", "Bob", "Chris", "Dod", "Evan", "Fint", "Gregg", "Brendan", "Roy", "Susan"};

struct pscore
{
    int num;
    char nme[10];
    float sc1;
    float sc2;
    float sum;
};

typedef struct pscore score;//typedef로 편리한 구조체 사용

int main(int argc, char *argv[])
{
    srand(time(NULL));//generator 수행마다 seed를 다르게 줌

    score st[1001];//index 1000까지의 학생 구조체 생성
    int randNum;
    FILE *fp;

    //printf("size : %ld\n", sizeof(score));

    if(argc != 2)
    {
        printf("< Usage ./%s filename_for_write >\n", __FILE__);
        return 1;
    }

    if((fp = fopen(argv[1] , "wb")) == NULL)//이진 파일에 binary write
    {
        perror("Open");
        exit(1);
    }

    randNum = rand() % 1001; //임의의 학생 수 생성

    for(int i=0; i<randNum; i++)
    {
        st[i].num = 201000000 + (rand() % 1000000);//학번 2010***** ~ 2019***** 생성
        strcpy(st[i].nme, names[rand() % 10]);//이름 생성
        st[i].sc1 = rand() % 100;//점수1 0~100 생성
        st[i].sc2 = rand() % 100;//점수2 0~100 생성
        st[i].sum = st[i].sc1 + st[i].sc2;// 합 계산

        fwrite(&st[i], sizeof(score), 1, fp);//구조체를 file에 이진 형태로 작성
    }

    fclose(fp);
    return 0;
}