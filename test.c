#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Fork
#include <sys/types.h>  // Semaphores
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/wait.h>


int i=0;

void fonction(){
int pid;
int j;
for(j=0;j<4;j++){
switch(pid=fork()){
case 0:
	i++;
	printf("fils %d\n",i);
	exit(0);
	break;
case -1:
	break;
default:
	//i++;
	printf("pere %d\n",i);
}
}
sleep(1);
}
int main(){

fonction();
return 0;
}
