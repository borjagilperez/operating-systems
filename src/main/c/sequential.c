/**
 * @brief Description...
 *
 * Long description...
 * @file sequential.c
 * @author borjagilperez at github.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int nproc, i = 0, status;
    pid_t pid, child_pid;

    if(argc != 2) {
        fprintf(stdout, "./sequential <#children>\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "\tRoot PID\t%d\n", getpid());
    if(sscanf(argv[1], "%d", &nproc) != 1) {
        fprintf(stdout, "Error in sscanf\n");
        exit(EXIT_FAILURE);
    }

    do {
        pid = fork();
        if(pid == -1) {
            perror("Error in fork");
            exit(EXIT_FAILURE);
        }
        if(pid == 0) { /* child */
            fprintf(stdout, "i %d\tChild PID\t%d\t\tPPID\t\t%d\n", i, getpid(), getppid());
        } else { /* parent */
            fprintf(stdout, "i %d\tParent PID\t%d\t\tChild PID\t%d\n", i, getpid(), pid);
        }
    } while(pid == 0 && i++ < nproc-1);

    sleep(5); /* simulation of the time an algorithm takes to run */
    fprintf(stdout, "\tPID running\t%d\t\tvar. 'pid' =\t%d\n", getpid(), pid);
    sleep(5); /* simulation of the time an algorithm takes to run */

    if(pid > 0) {
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
    } else {
        fprintf(stdout, "\n\tPID\t\t%d does not wait for any child\n", getpid());
    }

    fprintf(stdout, "\n\tPID\t\t%d finished\n", getpid());
    exit(EXIT_SUCCESS);
}
