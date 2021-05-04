/**
 * @brief Description...
 *
 * Long description...
 * @file orphan.c
 * @author borjagilperez at github.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t pid;

    fprintf(stdout, "Root PID\t%d\n", getpid());
    pid = fork();
    if(pid == -1) {
        perror("Error in fork");
        exit(EXIT_FAILURE);
    }

    if(pid == 0) { /* child */
        fprintf(stdout, "Child PID\t%d\t\tPPID\t\t%d\n", getpid(), getppid());
        sleep(16); /* simulation of the time an algorithm takes to run */
        fprintf(stdout, "\nChild PID\t%d\t\tPPID\t\t%d\n", getpid(), getppid());
    } else { /* parent */
        sleep(8); /* simulation of the time an algorithm takes to run */
        fprintf(stdout, "Parent PID\t%d\t\tChild PID\t%d\n", getpid(), pid);
    }

    /* code executed by parent and child */
    fprintf(stdout, "\nPID\t\t%d finished\n", getpid());
    exit(EXIT_SUCCESS);
}
