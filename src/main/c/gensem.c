/**
 * @brief Description...
 *
 * Long description...
 * @file gensem.c
 * @author borjagilperez at github.com
 */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define PROJ_ID 3
#define NSEMS 1

int main(int argc, char *argv[]) {
    int gens, scap, semid, i, nchildren = 0, status;
    struct sembuf sops;
    pid_t root_pid, pid = -1, child_pid;
    key_t skey;

    if(argc != 3) {
        fprintf(stdout, "./gensem <gens> <sem_capacity>\n");
        exit(EXIT_FAILURE);
    }
    if(sscanf(argv[1], "%d", &gens) != 1) {
        fprintf(stdout, "Error in sscanf\n");
        exit(EXIT_FAILURE);
    }
    if(sscanf(argv[2], "%d", &scap) != 1) {
        fprintf(stdout, "Error in sscanf\n");
        exit(EXIT_FAILURE);
    }
    if(gens < 1  || scap < 1) {
        fprintf(stdout, "Both the exponent and the semaphore capacity must be greater than 0\n");
        exit(EXIT_FAILURE);
    }

    /* request a key for semaphores */
    if((skey = ftok(argv[0], PROJ_ID)) == -1) {
        perror("Error in ftok");
        exit(EXIT_FAILURE);
    }
    /* create a set with one semaphore */
    if((semid = semget(skey, NSEMS, IPC_CREAT|IPC_EXCL|SHM_R|SHM_W)) == -1) {
        perror("Error in semget");
        /* remove semid if it already exists */
        if(errno == EEXIST) {
            semid = semget(skey, NSEMS, SHM_R|SHM_W);
            semctl(semid, 0, IPC_RMID, 0);
            fprintf(stdout, "Previous semid %d removed\n", semid);
            if((semid = semget(skey, NSEMS, IPC_CREAT|IPC_EXCL|SHM_R|SHM_W)) == -1) {
                perror("Error in semget");
                exit(EXIT_FAILURE);
            }
        } else {
            exit(EXIT_FAILURE);
        }
    }

    /* open semaphore #0 that allows access to a maximum of 'scap' processes */
    if(semctl(semid, 0, SETVAL, scap) == -1) {
        perror("Error in semctl");
        exit(EXIT_FAILURE);
    }

    root_pid = getpid();
    fprintf(stdout, "\tRoot PID\t%d\t\t\tS0 free: %d\tS0 waiting: %d\n\n", getpid(), semctl(semid, 0, GETVAL), semctl(semid, 0, GETNCNT));
    /* creation of generations,
     * running each generation 2^#gens processes */
    for(i = 0; i < gens; i++) {
        if((pid = fork()) == -1) {
            perror("Error in fork");
            exit(EXIT_FAILURE);
        }
        if(pid == 0) { /* child */
            fprintf(stdout, "i %d\tChild PID\t%d\t\tPPID\t\t%d\n", i, getpid(), getppid());
            nchildren = 0;
        } else { /* parent */
            fprintf(stdout, "i %d\tParent PID\t%d\t\tChild PID\t%d\n", i, getpid(), pid);
            nchildren += 1;
        }
    }

    /* =========================================================
     * decrease the semaphore capacity by one unit
     * =======================================================*/
    sops.sem_num = 0;
    sops.sem_op = -1;
    sops.sem_flg = SEM_UNDO;
    if(semop(semid, &sops, 1) == -1) {
        perror("Error in semop");
        exit(EXIT_FAILURE);
    }
    /*========================================================*/

    fprintf(stdout, "\tPID\t\t%d has entered\tS0 free: %d\tS0 waiting: %d\n", getpid(), semctl(semid, 0, GETVAL), semctl(semid, 0, GETNCNT));
    sleep((nchildren+1)*5);

    /* =========================================================
     * increase the semaphore capacity by one unit
     * =======================================================*/
    sops.sem_num = 0;
    sops.sem_op = 1;
    sops.sem_flg = SEM_UNDO;
    if(semop(semid, &sops, 1) == -1) {
        perror("Error in semop");
        exit(EXIT_FAILURE);
    }
    /*========================================================*/

    /* each process that has become a parent waits for as many child processes as it has */
    if(pid > 0) { /* parent */
        for(i = 0; i < nchildren; i++) {
            child_pid = waitpid(-1, &status, 0);
            if(child_pid == -1) {
                perror("Error in waitpid");
                exit(EXIT_FAILURE);
            }
            if(WIFEXITED(status) != 0) {
                if(WEXITSTATUS(status) == EXIT_SUCCESS) {
                    fprintf(stdout, "\tParent PID\t%d\t\tChild PID\t%d finished with code %d == EXIT_SUCCESS\n", getpid(), child_pid, WEXITSTATUS(status));
                } else if(WEXITSTATUS(status) == EXIT_FAILURE) {
                    fprintf(stdout, "\tParent PID\t%d\t\tChild PID\t%d finished with code %d == EXIT_FAILURE\n", getpid(), child_pid, WEXITSTATUS(status));
                } else {
                    fprintf(stdout, "\tParent PID\t%d\t\tChild PID\t%d finished with code %d\n", getpid(), child_pid, WEXITSTATUS(status));
                }
            } else {
                fprintf(stdout, "\tParent PID\t%d\t\tChild PID\t%d abnormally finished\n", getpid(), child_pid);
                exit(EXIT_FAILURE);
            }
        }
    }

    /* the root process is in charge of removing the semid,
     * since there are many parents in the genealogical tree */
    if(getpid() == root_pid) {
        fprintf(stdout, "\tRoot PID\t%d\t\t\tS0 free: %d\tS0 waiting: %d\n\n", getpid(), semctl(semid, 0, GETVAL), semctl(semid, 0, GETNCNT));
        if(semctl(semid, 0, IPC_RMID, 0) == -1) {
            perror("Error in semctl IPC_RMID");
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stdout, "\tPID\t\t%d finished\n", getpid());

    exit(EXIT_SUCCESS);
}
