/**
 * @brief Description...
 *
 * Long description...
 * @file intro.c
 * @author borjagilperez at github.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int status;
    pid_t pid, child_pid;

    fprintf(stdout, "Root PID\t%d\n", getpid());
    pid = fork();
    if(pid == -1) {
        perror("Error in fork");
        exit(EXIT_FAILURE);
    }
    
    if(pid == 0) { /* child */
        fprintf(stdout, "Child PID\t%d\t\tPPID\t\t%d\n", getpid(), getppid());
    } else { /* parent */
        fprintf(stdout, "Parent PID\t%d\t\tChild PID\t%d\n", getpid(), pid);
    }

    if(pid > 0) { /* parent */
        child_pid = waitpid(-1, &status, 0);
        if(child_pid == -1) {
            perror("Error in waitpid");
            exit(EXIT_FAILURE);
        }
        if(WIFEXITED(status) != 0) {
            if(WEXITSTATUS(status) == EXIT_SUCCESS) {
                fprintf(stdout, "\nParent PID\t%d\t\tChild PID\t%d finished with code %d == EXIT_SUCCESS\n", getpid(), child_pid, WEXITSTATUS(status));
            } else if(WEXITSTATUS(status) == EXIT_FAILURE) {
                fprintf(stdout, "\nParent PID\t%d\t\tChild PID\t%d finished with code %d == EXIT_FAILURE\n", getpid(), child_pid, WEXITSTATUS(status));
            } else {
                fprintf(stdout, "\nParent PID\t%d\t\tChild PID\t%d finished with code %d\n", getpid(), child_pid, WEXITSTATUS(status));
            }
        } else {
            fprintf(stdout, "\nParent PID\t%d\t\tChild PID\t%d abnormally finished\n", getpid(), child_pid);
            exit(EXIT_FAILURE);
        }
    }

    /* code executed by parent and child */
    fprintf(stdout, "\nPID\t\t%d finished\n", getpid());
    exit(EXIT_SUCCESS);
}
