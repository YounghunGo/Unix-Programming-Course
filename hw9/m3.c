#include <stdio.h>
#include <stdlib.h>

struct student_t{
    char id[10];
    int grade1, grade2, grade3;
    float avg;
    struct student_t *next;
};

typedef struct student_t student;

int main(int argc, char* argv[])
{
    FILE *fp;
    student *head, *cur, *prev;

    if(argc != 2)
    {
        printf("< Usage ./%s filename >\n", __FILE__);
        return 1;
    }

    if((fp = fopen(argv[1], "r")) == NULL)
    {
        perror("Open");
        exit(1);
    }

    cur = (student*) malloc(sizeof(student));
    head = cur;
    prev = cur;
    cur->next = NULL;
    printf("size : %ld", sizeof(student));
	while(fread(cur, 28, 1, fp) == 1)
	{
		fprintf(stdout, "%s %d %d %d %.2f\n", cur->id, cur->grade1, cur->grade2, cur->grade3, cur->avg);
	
        cur->next = (student*) malloc(sizeof(student));
        prev = cur;
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

    while(cur != NULL)
    {
        fprintf(stdout, "%s %d %d %d %.2f\n", cur->id, cur->grade1, cur->grade2, cur->grade3, cur->avg);

        prev = cur;
        cur = cur->next;
        free(prev);
    }

	fclose(fp);

	return 0;
}