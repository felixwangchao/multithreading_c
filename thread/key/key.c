/*
 * key: use a key 
 */

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_key_t key;

void fonction()
{
	int* p;
	p=pthread_getspecific(key);
	(*p)++;
}

void* thread(void* arg)
{
	int *p;
	p=(int*)malloc(sizeof(int));
	(*p)=(int)arg;
	pthread_setspecific(key,(void*)p);
	fonction();
	printf("%d:this is thread--value %d\n",(int)pthread_self(),*p);
	free(p);
	pthread_exit(0);
}


int main()
{
	//declaration de thread
	pthread_t tid1,tid2;

	//creation de key
	pthread_key_create(&key,NULL);

	//creation de thread
		pthread_create(&tid1,NULL,thread,(void*)10);
		pthread_create(&tid2,NULL,thread,(void*)20);
	
	sleep(1);
	
	return 0;
}
