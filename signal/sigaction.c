
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

struct sigaction action1;
struct sigaction action2;

void handler1(int sig)
{  
   printf("%d :je ne m'arrete pas~~~   \n", getpid()); 	
}

void handler2(int sig)
{
	signal(SIGINT,SIG_DFL);
	printf("you can ctrl+c now\n");
}

int main(){

	int i=0;
	
	action1.sa_handler = handler1;
	action1.sa_flags = 0;
	sigaction(SIGINT,&action1,NULL);

	action2.sa_handler = handler2;
	action2.sa_flags = 0;
	sigaction(SIGALRM,&action2,NULL);

	alarm(30);

	while(1){
		while(sleep(1)>0);
		printf("%d\n",i++);;
	}
	
	return 0;
}


