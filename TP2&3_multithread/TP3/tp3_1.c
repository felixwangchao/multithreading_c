/*
 * Programmation systeme et Multithreading TP3
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// Dimension pour la matrice et le vecteur
#define DIMENSION 2000

// Structure des arguments des threads
struct t_arg {
    int debut;
    int fin;
    int methode;
};

// Variables globales : Matrice et Vecteur
int mat[DIMENSION][DIMENSION];
int * vect;
int * next_vect;



void * thread (void * args)
{
    struct t_arg params = *(struct t_arg *) args;
    int lig, col;

    // Calcul des elements du nouveau vecteur
    if (params.methode == 1) {
        for (lig = params.debut; lig < params.fin; lig ++) {
            for (col = 0; col < DIMENSION; col ++) {
                next_vect[lig] += vect[col] * mat[lig][col];
            }
        }
    } else {
        for (col = params.debut; col < params.fin; col ++) {
            for (lig = 0; lig < DIMENSION; lig ++) {
                next_vect[col] += vect[lig] * mat[lig][col];
            }
        }
    }

    printf("Thread %d : Fini \n", (int) pthread_self());
    // Fin du thread
    pthread_exit(NULL);

    return 0;
}


int main (int argc, char * argv[])
{
    if (argc == 4) {
        int method = atoi(argv[1]);
        int nb_threads = atoi(argv[2]);
        int nb_iter = atoi(argv[3]);

        struct timeval debut, fin;
        float duree;
        gettimeofday(&debut, NULL);

        int lig, col;

        //mat = malloc(DIMENSION * DIMENSION * sizeof(int));
        vect = malloc(DIMENSION * sizeof(int));
        next_vect = malloc(DIMENSION * sizeof(int));

        printf("**** Init des structures globales ****\n");
        for (lig = 0; lig < DIMENSION; lig ++) {
            vect[lig] = 1;
            next_vect[lig] = 0;
            for (col = 0; col < DIMENSION; col ++) {
                mat[lig][col] = 1;
            }
        }
        printf("**** Init des structures globales OK ****\n");

        if (nb_threads != 0) {
            int lignes_par_thread = DIMENSION / nb_threads;

            // Déclaration des threads
            pthread_t tid[nb_threads];

            // Arguments des threads
            struct t_arg * args[nb_threads];

            printf("**** Init des args ****\n");
            for (lig = 0; lig < nb_threads; lig ++) {
                args[lig] = malloc(sizeof(struct t_arg));
                args[lig]->debut = lig * lignes_par_thread;
                args[lig]->methode = method;
                if (lig < nb_threads - 1) {
                    args[lig]->fin = (lig + 1) * lignes_par_thread;
                } else {
                    args[lig]->fin = DIMENSION;
                }
            }

            printf("**** Lancement des threads ****\n");
            for (col = 0; col < nb_iter; col ++) {
                printf("**** Iteration %d ****\n", col + 1);
                for (lig = 0; lig < nb_threads; lig ++) {
                    pthread_create(&tid[lig], NULL, thread, args[lig]);
                }

                for (lig = 0; lig < nb_threads; lig ++) {
                    pthread_join(tid[lig], NULL);
                }

                for (lig = 0; lig < DIMENSION; lig ++) {
                    vect[lig] = next_vect[lig];
                    next_vect[lig] = 0;
                }
            }

            gettimeofday(&fin, NULL);

            duree = (fin.tv_sec - debut.tv_sec) * 1000;
            duree += (fin.tv_usec - debut.tv_usec) / 1000;

            printf("Temps d'exection : %f\n", duree);
            // Affichage du vecteur résultat
            printf("**** Affichage du vecteur résultat ****\n");
            for (lig = 0; lig < DIMENSION; lig ++) {
//                printf("%d : %d\n", lig, vect[lig]);
            }
        }

    } else {
        perror("Pas le bon nb d'args\n");
    }

    return 0;
}
