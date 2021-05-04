/**
 * @brief Description...
 *
 * Long description...
 * @file generations.c
 * @author borjagilperez at github.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int gens, i, status, nchildren = 0;
    pid_t pid = -1, child_pid;

    if(argc != 2) {
        fprintf(stdout, "./generations <gens>\n");
        exit(EXIT_FAILURE);
    }
    if(sscanf(argv[1], "%d", &gens) != 1) {
        fprintf(stdout, "Error in sscanf\n");
        exit(EXIT_FAILURE);
    }
    if(gens < 1) {
        fprintf(stdout, "The exponent must be greater than 0\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "\tRoot PID\t%d\n", getpid());
    for(i = 0; i < gens; i++) {
        pid = fork();
        if(pid == -1) {
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

    /* each process that has become a parent waits for as many child processes as it has */
    if(pid > 0) { /* parent */
        sleep(5); /* simulation of the time an algorithm takes to run */
        for(i = 0; i < nchildren; i++) {
            child_pid = waitpid(-1, &status, 0);
            if(child_pid == -1) {
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
        }
    }

    fprintf(stdout, "\n\tPID\t\t%d finished\n", getpid());
    exit(EXIT_SUCCESS);
}
