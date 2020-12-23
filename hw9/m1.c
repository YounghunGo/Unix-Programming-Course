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
	student *data;
	

	if(argc != 2)
	{
		printf("< Usage: ./%s filename >\n", __FILE__);
		return 1;
	}

	if((fp = fopen(argv[1], "r")) == NULL)
	{
		perror("Open");
		exit(1);
	}

	data = (student*) malloc(sizeof(student));

	while(fread(data, sizeof(student), 1, fp) == 1)
	{
		fprintf(stdout, "%s %d %d %d %.2f\n", data->id, data->grade1, data->grade2, data->grade3, data->avg);
	}
	
	free(data);

	fclose(fp);

	sleep(20);
	return 0;
}
