#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>


int main()
{	int i,time;
	FILE *p;
	char buf[100];
	char* exec[3];

	for(i=0;i<3;i++)
	exec[i] = (char*)malloc(sizeof(char)*20);

	
	p = fopen("a.txt","r");


	while(fgets(buf,100,p)!=NULL){
		sscanf(buf,"%s\t%s\t%s\n",exec[0],exec[1],exec[2]);
		time = atoi(exec[2]);
		
		for(i=0;i<time;i++)
		{
			switch(fork()){
			case -1:
					perror("error");
					break;
			case 0:
					printf("declencher\n");
					execl(exec[0],exec[1],NULL);
					exit(0);
					break;
			default:
					break;	}	
		}	
		
	}
	
	
	
	
//waitpid(-1,NULL,0);
sleep(10);
return 0;
}
