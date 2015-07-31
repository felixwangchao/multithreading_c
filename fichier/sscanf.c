#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
	char* prog="temperature temperature 5";
	char* exec[3];
	int i;
	for(i=0;i<3;i++)
	exec[i] = (char*)malloc(sizeof(char)*20);
	sscanf(prog,"%s\t%s\t%s\n",exec[0],exec[1],exec[2]);
	printf("%s\n",exec[0]);	
	printf("%s\n",exec[1]);	
	printf("%s\n",exec[2]);	

	return 0;
}
