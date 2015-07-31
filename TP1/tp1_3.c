/* 
 * Multithreading TP1
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main (int argc, char * argv[])
{
    int token = 0;
    int taille_tube = 10;
    int nb_loop;
    int tube_ER, tube_RE;

    if (argc == 2) {
        // Processus emetteur

        nb_loop = atoi(argv[1]);
        printf("NB loop : %d\n", nb_loop);

        int temps_exe, debit;
        struct timeval td, tf;

        // Creation du tube Emetteur -> recepteur
        char buf_p[taille_tube];
        mkfifo("/tmp/tube_em_re", S_IRUSR | S_IWUSR);
        mkfifo("/tmp/tube_re_em", S_IRUSR | S_IWUSR);

        tube_ER = open("/tmp/tube_em_re", O_WRONLY);
        tube_RE = open("/tmp/tube_re_em", O_RDONLY);

        sprintf(buf_p, "%d", nb_loop);
        write(tube_ER, buf_p, taille_tube);

        gettimeofday(&td);

        while (token < nb_loop) {
            sprintf(buf_p, "%d", ++token);
            //printf("%d : Pere w %s\n", getpid(), buf_p);
            write(tube_ER, buf_p, taille_tube);
            read(tube_RE, buf_p, taille_tube);
            token = atoi(buf_p);
            //printf("%d : Pere r %d\n", getpid(), token);
        }

        gettimeofday(&tf);
        temps_exe = (int) ((tf.tv_sec - td.tv_sec) * 1000000 + (tf.tv_usec - td.tv_usec));
        debit = (1000 * 2 * 4 * 8) * pow(10.0, 6.0) / (temps_exe);

        // Affichage du debit en Mb/s
        printf("%d : Pere fini, debit = %d\n", getpid(), debit / (1024 * 1024));

    } else {

        char buf_f[taille_tube];

        printf("Recepteur\n");

        tube_ER = open("/tmp/tube_em_re", O_RDONLY);
        tube_RE = open("/tmp/tube_re_em", O_WRONLY);

        read(tube_ER, buf_f, taille_tube);
        nb_loop = atoi(buf_f);

        printf("NB loop : %d\n", nb_loop);

        while (token < nb_loop) {
            read(tube_ER, buf_f, taille_tube);
            token = atoi(buf_f);
            //printf("%d : Fils r %d\n", getpid(), token);
            sprintf(buf_f, "%d", ++token);
            write(tube_RE, buf_f, taille_tube);
            //printf("%d : Fils w %s\n", getpid(), buf_f);
        }

    }
    return 0;

}


