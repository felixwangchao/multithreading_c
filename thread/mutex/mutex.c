/*
 * mutex: create a mutex
 */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
int x;

void* thread(void* arg)
{
	int i;
	
	for(i=0;i<10;i++)
	{
		sleep(1);
		pthread_mutex_lock(&mutex);
		x=10;
		x=100/x;
		printf("x=%d in thread\n",x);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

int main()
{
	int i;
	pthread_t tid;
	pthread_create(&tid,NULL,thread,NULL);
	for(i=0;i<10;i++)
	{
		sleep(1);
		pthread_mutex_lock(&mutex);
		x=0;
		printf("x=%d in main\n",x);
		pthread_mutex_unlock(&mutex);
	}
		return 0;
}
