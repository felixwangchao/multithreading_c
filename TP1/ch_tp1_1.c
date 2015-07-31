/*
 *  multithread TP1 version de Chao
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int main()
{
	//delcaration de pid sigset et action de signal
	int pid;
	sigset_t mask;

	//fork
	switch(pid=fork())
	{
		//fils
		case 0:
					printf("fils demarre\n");
				
					//Reglage du masque de signal
					sigfillset(&mask);
					sigdelset(&mask,SIGUSR1);
					sigprocmask(SIG_SETMASK,&mask,NULL);	

					while(1)
					{
						sleep(1);
						printf("%d:fils attente\n",getpid());
					}			
					break;

		//cas erreur
		case -1:
					perror("Erreur creation fils\n");
					break;
		
		//pere
		default:
					printf("pere demarre\n");

					//Reglage du masque de signal
					sigfillset(&mask);
					sigdelset(&mask,SIGUSR1);
					sigprocmask(SIG_SETMASK,&mask,NULL);
			
					sleep(10);
					kill(pid,SIGUSR1);
					
					while(1)
					{	
						sleep(1);
						printf("%d:pere attente\n",getpid());
					}					
					break;	
	}
return 0;
}



