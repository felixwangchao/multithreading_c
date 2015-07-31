//********************************************
//*				examen multithreaed			 *		 
//*		  			WANG Chao				 *
//*				 Main processus				 *
//********************************************

//head
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Fork
#include <sys/types.h>  // Semaphores
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/wait.h>

int key_mem=123,id_mem;
	int *mem_main=NULL;

void CreerProcess()
{
	FILE *p;

	int i,time;
	char buf[100];
	char* exec[3];
	
	if ((id_mem = shmget(key_mem, sizeof(int), 0600 | IPC_CREAT)) == -1) {
        perror("Erreur creation memoire partagee\n");
    }

	if ((mem_main = shmat(id_mem, NULL, 0)) == NULL) {
            perror("Erreur attachement memoire fils\n");
        }

	*mem_main = 0;

	//allocation des memeoire
	for(i=0;i<3;i++)
	exec[i] = (char*)malloc(sizeof(char)*20);

	//open the file
	p = fopen("file.txt","r");

	while(fgets(buf,100,p)!=NULL){
		sscanf(buf,"%s\t%s\t%s\n",exec[0],exec[1],exec[2]);
		time = atoi(exec[2]);
		
		for(i=0;i<time;i++)
		{
			switch(fork()){
			case -1:
					perror("error");
					break;
			case 0:
					execl(exec[0],exec[1],NULL);
					exit(0);
					break;
			default:
					break;	}	
		}	
		
	}
	
}

int main()
{
	CreerProcess();// fonction pour creer des process
	
	waitpid(-1,NULL,0);

	sleep(4);	
	printf("%d\n",*mem_main);
	
	shmctl(id_mem, IPC_RMID, 0);
	return 0;
}
