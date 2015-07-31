	/*
 * once: use once to initial one time
 */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

pthread_once_t once=PTHREAD_ONCE_INIT;

void fonction_init()
{
	int i;
	int a[10];
	for(i=0;i<10;i++)
	{
		a[i]=0;
		printf("a[%d] = %d\n",i,a[i]);
	}
	printf("initialisation is finished\n");
}

void* thread(void* arg)
{
	pthread_once(&once,fonction_init);
	pthread_exit(0);
}

int main()
{
	//declaration des variables
	int i;
	
	//declaration of threads
	pthread_t tid[3];

	//creation des threads
	pthread_create(&tid[0],NULL,thread,NULL);
	pthread_create(&tid[1],NULL,thread,NULL);
	pthread_create(&tid[2],NULL,thread,NULL);
	
	//attente tous les threads sont finis	
	for(i=0;i<3;i++)
	{
		pthread_join(tid[i],NULL);
	}
	
	printf("main fin\n");
	return 0;
}
