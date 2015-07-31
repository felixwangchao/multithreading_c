#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

void* fonction(void* arg)
{
	printf("a\n");
	sleep(1);
	printf("aa\n");
	pthread_exit(0);
}


int main()
{
	pthread_t tid;
	pthread_create(&tid,NULL,fonction,NULL);
	pthread_create(&tid,NULL,fonction,NULL);

	sleep(2);
	return 0;
}
