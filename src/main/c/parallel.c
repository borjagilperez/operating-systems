/**
 * @brief Description...
 *
 * Long description...
 * @file parallel.c
 * @author borjagilperez at github.com
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int nproc, i = 0, status, t, tmp1, tmp2;
    pid_t pid, child_pid;

    if(argc != 2) {
        fprintf(stdout, "./parallel <#children>\n");
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

            /* algorithm simulation to observe each child running in a CPU */
            for(t = 0; t < (i+1)*10+(i+1)*2; t++) {
                for(tmp1 = 0; tmp1 < INT_MAX; tmp1++) {
                    tmp2 = tmp1/2;
                }
            }
            fprintf(stdout, "\ni %d\tChild PID\t%d\t\tPPID\t\t%d\ttmp2 %u\n", i, getpid(), getppid(), tmp2);

        } else { /* parent */
            fprintf(stdout, "i %d\tParent PID\t%d\t\tChild PID\t%d\n", i, getpid(), pid);
        }
    } while(pid > 0 && i++ < nproc-1);

    if(pid > 0) { /* parent */
        for(i = 0; i < nproc; i++) {
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
