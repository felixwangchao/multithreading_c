/*
 * cleanup: use a push and pop
 */

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

void handel(void* arg)
{
	printf("finished\n");
}

void* thread(void* arg)
{
	pthread_cleanup_push(handel,NULL);
	printf("fils\n");
	sleep(3);
	pthread_cleanup_pop(0);
	pthread_exit(0);
}


int main(int argc,char* argv[])
{
	//declaration de thread
	pthread_t tid;

	//creation de thread
		pthread_create(&tid,NULL,thread,NULL);
		sleep(1);
		
	  if(argc>1 && strcmp(argv[1],"1")==0)
		{
			pthread_cancel(tid);
		}
	sleep(1);
	return 0;
}
