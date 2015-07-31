/*
 * Programmation systeme et Multithreading examen2008 magicien
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Fork
#include <sys/types.h>  // Semaphores
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/wait.h>

typedef struct{
int pid;
int camp;
}mag;

mag* magicien

void CreerXmagiciens(){
	int i,force;
	for(i=0;i<5;i++)
	{
		switch(fork())
		{
			case -1:
				perror("Creation magicien fail\n");
				break;
			
			case 0:
				printf("%d:magicien cree\n",getpid());
				exit(0);
				break;
			default:
				if(i==4)
				{
					sleep(1);
					printf("%d:magicien Gray devenu magicien normal\n",getpid());
				}
				break;
		}
	}
}


int main(){
	//declaration des variables
	
	//creer magiciens
	CreerXmagiciens();
	waitpid(-1,NULL,0);
	return 0;
}
