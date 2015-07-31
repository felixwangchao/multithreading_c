/*
 * Programmation systeme et Multithreading pro1
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

struct patient{
	int pid;
	int ordre;
	int flag;
	int maladie;
};

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};

void handel(int sig)
{
	printf("\ntraite le patient\n");
	exit(0);
	//sleep(3);
}

void handel2(int sig)
{
	printf("specialiste consulte\n");
	sleep(5);
}

int main(){
	
 int i,pid_spec;
 int* record;
	
 struct patient* p;

 int id_mem;
 int key_mem=200;

 int id_record;
 int key_record=123;

 // Creation du semaphore
 int id; // Identifiant du semaphore
 int key=250;
 id = semget(key, 2, 0666 | IPC_CREAT);

 // Initialisation du semaphore
 union semun init; // Pour initialiser le semaphore
 init.val = 1;
 semctl(id, 0, SETVAL, init);

//changer la comportement de signal SIGUSR1 
 struct sigaction action;
	action.sa_handler=&handel;
	action.sa_flags=0;
	sigaction(SIGUSR1,&action,NULL);

//changer la comportement de signal SIGUSR2
 struct sigaction action2;
	action2.sa_handler=&handel2;
	action2.sa_flags=0;
	sigaction(SIGUSR2,&action2,NULL);

 // Buffer du semaphore pour definir son comportement
    struct sembuf bufsem_p;
    bufsem_p.sem_num = 0;
    bufsem_p.sem_flg = SEM_UNDO;
    bufsem_p.sem_op = -1; // P(S)

    struct sembuf bufsem_v;
    bufsem_v.sem_num = 0;
    bufsem_v.sem_flg = SEM_UNDO;
    bufsem_v.sem_op = 1; // V(S)
	
		struct sembuf bufsem_p2;
    bufsem_p.sem_num = 1;
    bufsem_p.sem_flg = SEM_UNDO;
    bufsem_p.sem_op = -1; // P(S)

    struct sembuf bufsem_v2;
    bufsem_v.sem_num = 1;
    bufsem_v.sem_flg = SEM_UNDO;
    bufsem_v.sem_op = 1; // V(S)


 //memoire partage cree
 if ((id_mem = shmget(key_mem, sizeof(struct patient)*10, 0666 | IPC_CREAT)) == -1) {
       perror("Erreur creation memoire partagee\n");
    }

 if((id_record = shmget(key_record,sizeof(int)*10,0666 | IPC_CREAT)) == -1){
			perror("Erreur creation memoire partagee\n");
		}
 

 
 //creation des 5 medecins
	for(i=0;i<5;i++)
		{
			switch(fork())
			{
				case -1:
								perror("fork fail\n");
								break;
				case	0:

								//memoire le pid de specialiste 
								pid_spec=getpid()+(4-i);

								//attachement de memoire partage	
								p=(struct patient*)shmat(id_mem,NULL,0);
								record=(int*)shmat(id_record,NULL,0);
				
										if(i==4)
								{

									//medecin specialiste
									printf("Medecin specialiste%d est libre\n",getpid());
									for(i=0;i<10;i++)
									{
								//s'il y'a pas de patient, reste attente
										while(((p+i)->pid)==0) sleep(1);

								//verifier est ce que ce patient est deja traite ou pas
										if((p+i)->flag == 0)
										{
											(p+i)->flag = 1;

											//debloque la thread client
											kill((p+i)->pid,10);
											printf("-------------------------\nMEDECIN Specialiste%d:appele numero %d\n-------------------------\n",getpid(),i+1);		
											while(sleep(40)>0);		
											semop(id, &bufsem_p, 1);
											*(record+i)=((p+i)->ordre);
											printf("\nMEDECIN Specialiste%d:ecrit le nom de patient %d\n\n",getpid(),*(record+i));
											//lache le semaphore   											
											semop(id, &bufsem_v, 1);
										}	
									}
									exit(0);					
								}


									//medecin generaliste
									printf("medecin%d est libre\n",getpid());
									for(i=0;i<10;i++)
									{
								//s'il y'a pas de patient, reste attente
										while(((p+i)->pid)==0) sleep(1);

								//verifier est ce que ce patient est deja traite ou pas
										if((p+i)->flag == 0)
										{
											(p+i)->flag = 1;

											//debloque la thread client
											kill((p+i)->pid,10);
											printf("-------------------------\nMEDECIN%d:appele numero %d\n-------------------------\n",getpid(),i+1);

											if((p+i)->maladie==1){
												semop(id, &bufsem_p2, 1);		
												kill(pid_spec,12);
												sleep(5);
												semop(id, &bufsem_v2, 1);														
												}		
											
											sleep(40);

											//prise le semaphore
											semop(id, &bufsem_p, 1);
											*(record+i)=((p+i)->ordre);
											printf("\nMEDECIN%d:ecrit le nom de patient %d\n\n",getpid(),*(record+i));
											//lache le semaphore   											
											semop(id, &bufsem_v, 1);
										}
									}
								exit(0);
								break;
				default:
								
								break;			
			}
		}	
	
 
 //creation des patients
		for(i=0;i<10;i++)
		{
			sleep(6);
			switch(fork())
			{
				case -1:
								perror("fork fail\n");
								break;
				case	0:
								p=(struct patient*)shmat(id_mem,NULL,0);
								printf("\n\n*Patient%d:ticket:%d\n\n",getpid(),i+1);
								(p+i)->pid=getpid();
								(p+i)->ordre=i+1;
								(p+i)->flag = 0;
								if(i==3||i==5||i==7)
								(p+i)->maladie=1;
								else
								(p+i)->maladie=0;
								
								while(1);
								
								exit(0);
								break;
				default:
								
								break;			
			}
		}	
	
/*	p=(struct patient*)shmat(id_mem,NULL,0);
	for(i=0;i<10;i++)
	{
		printf("client:%d  pris ordre:%d\n",p[i].pid,p[i].ordre);
	}
*/
	waitpid(-1,NULL,0);
	shmctl(id_mem,IPC_RMID,0);	
	shmctl(id_record,IPC_RMID,0);		
	return 0;
}
