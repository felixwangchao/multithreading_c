/*
 * thread: use join without argument
 */

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

void* thread(void* arg)
{
	printf("%d:this is thread\n",(int)pthread_self());
	sleep(3);
	pthread_exit(0);
}


int main()
{
	//declaration de thread
	pthread_t tid;

	//creation de thread
		pthread_create(&tid,NULL,thread,NULL);
	
	pthread_join(tid,NULL);
	printf("%d:the is finished \n",(int)pthread_self());
	return 0;
}
