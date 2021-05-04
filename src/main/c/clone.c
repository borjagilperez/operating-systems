/**
 * @brief Description...
 *
 * Long description...
 * @file clone.c
 * @author borjagilperez at github.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int ca_lenght, n, status;
    char *pca = NULL;
    pid_t pid, child_pid;

    if(argc != 4) {
        fprintf(stdout, "./clone <length> <char array> <number>\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "Root PID\t%d\n", getpid());
    if(sscanf(argv[1], "%d", &ca_lenght) != 1) {
        fprintf(stdout, "Error in sscanf\n");
        exit(EXIT_FAILURE);
    }
    if(sscanf(argv[3], "%d", &n) != 1) {
        fprintf(stdout, "Error in sscanf\n");
        exit(EXIT_FAILURE);
    }
    pca = (char *)malloc(ca_lenght*sizeof(char));
    if(pca == NULL) {
        fprintf(stdout, "Error in malloc\n");
        exit(EXIT_FAILURE);
    }
    strcpy(pca, argv[2]);

    pid = fork();
    if(pid == -1) {
        perror("Error in fork");
        free((char *)pca);
        exit(EXIT_FAILURE);
    }

    if(pid == 0) { /* child */
        fprintf(stdout, "Child PID\t%d\t\tPPID\t\t%d\n", getpid(), getppid());
        sleep(2); /* simulation of the time an algorithm takes to run */
        fprintf(stdout, "Enter a new string of characters: ");
        if(fgets(pca, ca_lenght, stdin) == NULL) {
            fprintf(stdout, "Error in fgets\n");
            free((char *)pca);
            exit(EXIT_FAILURE);
        }
        //pca[strlen(pca)-1] = pca[strlen(pca)];
        n += 2;
    } else { /* parent */
        fprintf(stdout, "Parent PID\t%d\t\tChild PID\t%d\n", getpid(), pid);
    }

    if(pid > 0) { /* parent */
        child_pid = waitpid(-1, &status, 0);
        if(child_pid == -1) {
            perror("Error in waitpid");
            free((char *)pca);
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
            fprintf(stdout, "\nParent PID\t%d\t\tChild PID\t%d abnormally finished\n", getpid(), child_pid);
            free((char *)pca);
            exit(EXIT_FAILURE);
        }
    }

    /* code executed by parent and child */
    fprintf(stdout, "\nPID\t\t%d finished\t\tCharacter array: %s\tNumber: %d\n", getpid(), pca, n);
    free((char *)pca);
    exit(EXIT_SUCCESS);
}
