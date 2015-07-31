//********************************************
//*				examen multithreaed			 *		 
//*		  			WANG Chao				 *
//*				 Main processus				 *
//********************************************

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Fork
#include <sys/types.h>  // Semaphores
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/wait.h>

int main()
{
	//declaration des variable
	int key_mem=123,id_mem;
	int *mem_take=NULL;

	//obtenir le id de memeoire partage
	if ((id_mem = shmget(key_mem, sizeof(int), 0600 | IPC_CREAT)) == -1) {
        perror("Erreur creation memoire partagee\n");
    }

	if ((mem_take = shmat(id_mem, NULL, 0)) == NULL) {
            perror("Erreur attachement memoire fils\n");
        }
	
	sleep(1);
	(*mem_take)++;
	printf("take finished\n");
	
	return 0;

}
