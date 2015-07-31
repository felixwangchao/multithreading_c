/*
 * thread: use join to transport the variable
 */

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

//int a=10;


void* thread(void* arg)
{
	printf("%d:this is thread\n",(int)pthread_self());
	sleep(3);
	int a=10,*p;
	//int *p;
	p=(int*)malloc(sizeof(int));
	*p=a;
	pthread_exit(p);
}


int main()
{
	//variable
	void* recu;
	
	//declaration de thread
	pthread_t tid;

	//creation de thread
	pthread_create(&tid,NULL,thread,NULL);
	
	//attente le thread fini
	pthread_join(tid,&recu);
	printf("%d:the is finished %d\n",(int)pthread_self(),*(int*)recu);
	free(recu);
	return 0;
}
