#include<string.h>
#include<stdio.h>

int main()
{
	char s[] = "temperature  temperature  5";
	char *cut = " ";
	char *p;

	printf("%s\n",strtok(s,cut));
	while((p=strtok(NULL,cut)))
		printf("%s\n",p);
		printf("\n");
	return 0;
}
