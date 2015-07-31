/*
 * mutex: use trylock --- probleme of Joker
 */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2=PTHREAD_MUTEX_INITIALIZER;
int px=1,py=2;

//thread1: use only px
void* thread1(void* arg)
{
	int i;
	
	for(i=0;i<10;i++)
	{
		sleep(1);
		pthread_mutex_lock(&mutex1);
		printf("thread1:the value of px is %d\n",px);
		pthread_mutex_unlock(&mutex1);
	}
	pthread_exit(NULL);
}

//thread2: use only py
void* thread2(void* arg)
{
	int i;
	
	for(i=0;i<10;i++)
	{
		sleep(1);
		pthread_mutex_lock(&mutex2);
		printf("thread2:the value of py is %d\n",py);
		pthread_mutex_unlock(&mutex2);
	}
	pthread_exit(NULL);
}

//thread3: use px and py
void* thread3(void* arg)
{
	int i;
	
	for(i=0;i<10;i++)
	{
		sleep(1);
		//first: lock the mutex1
		pthread_mutex_lock(&mutex1);

		//second: try the mutex2

		if(pthread_mutex_trylock(&mutex2)==0)
			{
				printf("thread3:px=%d py=%d\n",px,py);
				pthread_mutex_unlock(&mutex1);
				pthread_mutex_unlock(&mutex2);
			}//if yes: work
		else
			{				
				pthread_mutex_unlock(&mutex1);
				i--;
			}//if no: unlock mutex1
	}
	pthread_exit(NULL);
}

int main()
{	
	//declaration
	int i;
	pthread_t tid[3];

	//create threads
	pthread_create(&tid[0],NULL,thread1,NULL);
	pthread_create(&tid[1],NULL,thread2,NULL);
	pthread_create(&tid[2],NULL,thread3,NULL);

	//wait all of threads
	for(i=0;i<3;i++)
	{
		pthread_join(tid[i],NULL);
	}

	printf("all of thread is finished\n");
	return 0;
}
