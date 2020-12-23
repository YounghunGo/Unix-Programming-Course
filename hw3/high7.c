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
		printf("< Usage: ./high7 filename >\n");
		return 1;
	}

	if((fp = fopen(argv[1], "w")) == NULL)
	{
		perror("Open");
		exit(1);
	}

	for(i=0; i<3; i++)
	{
		if(fscanf(stdin, "%s %d %d %d", data[i].id, &data[i].grade1, &data[i].grade2, &data[i].grade3) > 0)
		{
			data[i].avg = (data[i].grade1+data[i].grade2+data[i].grade3)/3;
			fwrite(&data[i], sizeof(student), 1, fp);
		}
		else
			break;
	}

	fclose(fp);

	return 0;
}
