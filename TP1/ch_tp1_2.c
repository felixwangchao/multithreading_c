/*
 * multithread TP1:tube ordinaire -version de Chao
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<memory.h>
#include <math.h>
#include <time.h>


//********************************************
//*				    Main 					 *			
//*	 1.cree deux tubes pour deux direction 	 *
//*	 2.cree deux processus					 *
//********************************************

int main()
{
	//delcaration de pid sigset et action de signal 
	int pid;
	int token=0;
	
	int temps_exe, debit;
  	struct timeval td, tf;

	//creation des pipe
	int pipe_pf[2]; //direction pere a fils
	int pipe_fp[2]; //direction fils a pere
	int size=10;    // size de buffer
	char buf_pf[size]; //buffer de pere a fils
	char buf_fp[size]; //buffer de fils a pere

    //netoyer la memeoire
	memset(buf_pf,'\0',size); 
	memset(buf_fp,'\0',size); 
	if(pipe(pipe_pf)!=0)
		perror("Erreur creation pipe\n");
	if(pipe(pipe_fp)!=0)
		perror("Erreur creation pipe\n");


	//fork
	switch(pid=fork())
	{
		//fils
		case 0:
					printf("%d:fils demarre\n",getpid());
					
					while(token<1000)
					{
					read(pipe_pf[0],buf_pf,size);
					//printf("%d : Fils r %s\n", getpid(), buf_pf);
					token=atoi(buf_pf);
					sprintf(buf_fp, "%d", ++token);
					write(pipe_fp[1],buf_fp,size);
					//printf("%d : Fils w %s\n", getpid(),buf_fp);
					}			
					break;

		//cas erreur
		case -1:
					perror("Erreur creation fils\n");
					break;
		
		//pere
		default:
					printf("%d:pere demarre\n",getpid());
					gettimeofday(&td,NULL);
					while(token<1000)
					{	
						
						sprintf(buf_pf, "%d", ++token);
						write(pipe_pf[1],buf_pf,size);
						//printf("%d : Pere w %s\n", getpid(), buf_pf);
						read(pipe_fp[0],buf_fp,size);
						token=atoi(buf_fp);
						//printf("%d : Pere r %d\n", getpid(), token);
					}				

					gettimeofday(&tf,NULL);
       		temps_exe = (int) ((tf.tv_sec - td.tv_sec) * 1000000 + (tf.tv_usec - td.tv_usec));
        	debit = (1000 * 2 * 4 * 8) * pow(10.0, 6.0) / (temps_exe);

        	// Affichage du debit en Mb/s
        	printf("%d : Pere fini, debit = %d\n", getpid(), debit / (1024 * 1024));
	
					break;	
	}
return 0;
}

