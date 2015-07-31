/*
 * thread: create a thread
 */

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

void* thread(void* arg)
{
	printf("%d:this is thread\n",(int)pthread_self());
	pthread_exit(0);
}


int main()
{
	//declaration de thread
	pthread_t tid;

	//creation de thread
		pthread_create(&tid,NULL,thread,NULL);

	sleep(1);
	printf("%d:this is pere\n",(int)pthread_self());
	return 0;
}
