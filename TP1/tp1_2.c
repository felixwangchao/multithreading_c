/* 
 * Multithreading TP1
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include<sys/time.h>
#include <time.h>

void handler_child(int sig)
{
   printf("%d : Mon fils est mort\n", getpid()); 
}



int main ()
{
    int pid = 0;

    int temps_exe, debit;
    struct timeval td, tf;

    int token = 0;
    
    // Creation des tubes pere -> fils et fils -> pere
    int tube_pf[2];
    int tube_fp[2];
    int taille_tube = 10;
    char buf_p[taille_tube];
    char buf_f[taille_tube];
    pipe(tube_pf);
    pipe(tube_fp);

    switch (pid = fork()) {
    case -1 :
        perror("Erreur creation fils\n");
        exit(-1);
        break;
    case 0 :
        printf("%d : Fils démarré\n", getpid());

        while (token < 1000) {
            read(tube_pf[0], buf_f, taille_tube);
            token = atoi(buf_f);
            //printf("%d : Fils r %d\n", getpid(), token);
            sprintf(buf_f, "%d", ++token);
            write(tube_fp[1], buf_f, taille_tube);
            //printf("%d : Fils w %s\n", getpid(), buf_f);
        }

        printf("%d : Fils fini\n", getpid());

        break;
    default :
        printf("%d : Pere continue\n", getpid());
        
        gettimeofday(&td,NULL);

        while (token < 1000) {
            sprintf(buf_p, "%d", ++token);
            //printf("%d : Pere w %s\n", getpid(), buf_p);
            write(tube_pf[1], buf_p, taille_tube);
            read(tube_fp[0], buf_p, taille_tube);
            token = atoi(buf_p);
            //printf("%d : Pere r %d\n", getpid(), token);
        }

        gettimeofday(&tf,NULL);
        temps_exe = (int) ((tf.tv_sec - td.tv_sec) * 1000000 + (tf.tv_usec - td.tv_usec));
        debit = (1000 * 2 * 4 * 8) * pow(10.0, 6.0) / (temps_exe);

        // Affichage du debit en Mb/s
        printf("%d : Pere fini, debit = %d\n", getpid(), debit / (1024 * 1024));

     
    }
   return 0;
}


