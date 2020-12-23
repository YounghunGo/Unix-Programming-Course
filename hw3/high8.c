#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct 
{
	char id[10];
	int grade1, grade2, grade3;
	float avg;
} student;

int main(int argc, char* argv[])
{
	FILE* fp;
	int i;
	student data[3];
	

	if(argc != 2)
	{
		printf("< Usage: ./high8 filename >\n");
		return 1;
	}

	if((fp = fopen(argv[1], "r")) == NULL)
	{
		perror("Open");
		exit(1);
	}

	fprintf(stdout, "sizeof(student) = %ld\n", sizeof(student));

	while(fread(&data[i], sizeof(student), 1, fp) == 1)
	{
		fprintf(stdout, "%s %d %d %d %.2f\n", data[i].id, data[i].grade1, data[i].grade2, data[i].grade3, data[i].avg);
	}

	fclose(fp);

	return 0;
}
