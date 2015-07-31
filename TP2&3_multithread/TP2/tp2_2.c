/*
 * Programmation systeme et Multithreading TP2
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Fork
#include <sys/types.h>  // Semaphores
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>


union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};


int main ()
{
    int pid = 0;

    char chaine[10];

    // Creation des deux semaphores
    int id; // Identifiant des semaphores
    key_t key_sem;
    if ((id = semget(key_sem, 2, 0666 | IPC_CREAT)) == -1 ) {
        perror("Erreur creation semaphores\n");
    }

    printf("Semid : %d\n", id);

    // Initialisation des semaphores
    union semun init; // Pour initialiser le semaphore
    init.val = 0;
    semctl(id, 0, SETVAL, init);
    semctl(id, 1, SETVAL, init);

    // Buffer des semaphores pour definir P et V
    struct sembuf bufsem1_p;
    bufsem1_p.sem_num = 0;
    bufsem1_p.sem_flg = SEM_UNDO;
    bufsem1_p.sem_op = -1; // P(S)

    struct sembuf bufsem1_v;
    bufsem1_v.sem_num = 0;
    bufsem1_v.sem_flg = SEM_UNDO;
    bufsem1_v.sem_op = 1; // V(S)

    struct sembuf bufsem2_p;
    bufsem2_p.sem_num = 1;
    bufsem2_p.sem_flg = SEM_UNDO;
    bufsem2_p.sem_op = -1; // P(S)

    struct sembuf bufsem2_v;
    bufsem2_v.sem_num = 1;
    bufsem2_v.sem_flg = SEM_UNDO;
    bufsem2_v.sem_op = 1; // V(S)

    // Creation de la zone de memoire partagee
    int id_mem;
    key_t key_mem;
    if ((id_mem = shmget(key_mem, sizeof(int), 0600 | IPC_CREAT)) == -1) {
        perror("Erreur creation memoire partagee\n");
    }

    switch (pid = fork()) {
    case -1 :
        perror("Erreur creation fils\n");
        break;
    case 0 :
        printf("%d : Fils\n", getpid());
        int var_fils = 0;
        int * p;
        int * mem_fils = NULL;

        // Attachement de la memoire partagee
        if ((mem_fils = shmat(id_mem, NULL, 0)) == NULL) {
            perror("Erreur attachement memoire fils\n");
        }

        printf("%d : Coucou\n", getpid());

        while ((*mem_fils) < 1000) {
            semop(id, &bufsem2_p, 1);
            //var_fils = mem_fils;
            (*mem_fils) ++;
            //* mem_fils = * ((void*)(&var_fils));
            printf("%d : Var fils = %d\n", getpid(), *mem_fils); 
            semop(id, &bufsem1_v, 1);
        }

        break;
    default :
        printf("%d : Pere\n", getpid());
        int var_pere =0;
        int * mem_pere = NULL;
        
        // Attachement de la memeoire partagee
        if ((mem_pere = shmat(id_mem, NULL, 0)) == NULL) {
            perror("Erreur attachement memoire pere\n");
        }
        printf("%d : Coucou\n", getpid());
        (*mem_pere) = var_pere;

        printf("%d : Coucou\n", getpid());

        while ((*mem_pere) < 1000) {
            (*mem_pere) ++;
            //* mem_pere = * ((void*)(&var_pere));
            printf("%d : Var pere = %d\n", getpid(), *mem_pere);
            semop(id, &bufsem2_v, 1);
            semop(id, &bufsem1_p, 1);
            //var_pere = *((int*) mem_pere);
        }
    } 

    // Supression des semaphores
    semctl(id, 0, IPC_RMID, 0);
    semctl(id, 1, IPC_RMID, 0);

    // Suppression de la memoire partagee
    shmctl(id_mem, IPC_RMID, 0);

    return 0;
}
