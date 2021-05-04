/**
 * @brief Description...
 *
 * Long description...
 * @file alternate.c
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

#define PROJ_ID 1
#define NSEMS 1

int main(int argc, char *argv[]) {
    int semid, i = 10, status;
    struct sembuf sops;
    pid_t pid, child_pid;
    key_t skey;

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

    /* open semaphore #0 */
    if(semctl(semid, 0, SETVAL, 1) == -1) {
        perror("Error in semctl");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "\tRoot PID\t%d\n", getpid());
    if((pid = fork()) == -1) {
        perror("Error in fork");
        exit(EXIT_FAILURE);
    }
    if(pid == 0) { /* child */
        //sleep(2); /* sleep #1 */
        while(i > 0) {
            /* =========================================================
             * close semaphore #0
             * =======================================================*/
            sops.sem_num = 0;
            sops.sem_op = -1;
            sops.sem_flg = SEM_UNDO;
            if(semop(semid, &sops, 1) == -1) {
                perror("Error in semop");
                exit(EXIT_FAILURE);
            }
            /*========================================================*/

            fprintf(stdout, "i %d\tChild PID\t%d\n", i--, getpid());
            //sleep(1); /* sleep #2 */

            /* =========================================================
             * open semaphore #0
             * =======================================================*/
            sops.sem_num = 0;
            sops.sem_op = 1;
            sops.sem_flg = SEM_UNDO;
            if(semop(semid, &sops, 1) == -1) {
                perror("Error in semop");
                exit(EXIT_FAILURE);
            }
            /*========================================================*/
        }
    } else { /* parent */
        //sleep(2); /* sleep #3 */
        while(i > 0) {
            /* =========================================================
             * close semaphore #0
             * =======================================================*/
            sops.sem_num = 0;
            sops.sem_op = -1;
            sops.sem_flg = SEM_UNDO;
            if(semop(semid, &sops, 1) == -1) {
                perror("Error in semop");
                exit(EXIT_FAILURE);
            }
            /*========================================================*/

            fprintf(stdout, "i %d\tParent PID\t%d\n", i--, getpid());
            //sleep(1); /* sleep #4 */

            /* =========================================================
             * open semaphore #0
             * =======================================================*/
            sops.sem_num = 0;
            sops.sem_op = 1;
            sops.sem_flg = SEM_UNDO;
            if(semop(semid, &sops, 1) == -1) {
                perror("Error in semop");
                exit(EXIT_FAILURE);
            }
            /*========================================================*/
        }
    }

    if(pid > 0) { /* parent */
        if((child_pid = waitpid(-1, &status, 0)) == -1) {
            perror("Error in waitpid");
            exit(EXIT_FAILURE);
        }
        if(WIFEXITED(status) != 0) {
            if(WEXITSTATUS(status) == EXIT_SUCCESS) {
                fprintf(stdout, "\n\tParent PID\t%d\t\tChild PID\t%d finished with code %d == EXIT_SUCCESS\n", getpid(), child_pid, WEXITSTATUS(status));
            } else if(WEXITSTATUS(status) == EXIT_FAILURE) {
                fprintf(stdout, "\n\tParent PID\t%d\t\tChild PID\t%d finished with code %d == EXIT_FAILURE\n", getpid(), child_pid, WEXITSTATUS(status));
            } else {
                fprintf(stdout, "\n\tParent PID\t%d\t\tChild PID\t%d finished with code %d\n", getpid(), child_pid, WEXITSTATUS(status));
            }
        } else {
            fprintf(stdout, "\n\tParent PID\t%d\t\tChild PID\t%d abnormally finished\n", getpid(), child_pid);
            exit(EXIT_FAILURE);
        }

        /* remove semid */
        if(semctl(semid, 0, IPC_RMID, 0) == -1) {
            perror("Error in semctl IPC_RMID");
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stdout, "\n\tPID\t\t%d finished\n", getpid());
    exit(EXIT_SUCCESS);
}
