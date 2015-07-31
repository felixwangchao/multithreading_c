#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{
	int i;
	char* argv[3];
	char* pf="temperature";

	argv = malloc(char**)malloc(sizeof(char)*60);

	for(i=0;i<3;i++)
	{	
		
		strcpy(argv[i],pf);
		printf("%s\n",argv[i]);
	}

	return 0;
}
