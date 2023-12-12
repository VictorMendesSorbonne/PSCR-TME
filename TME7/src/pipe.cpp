#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>


int main(int argc, char *argv[]) {

    int pipe_index = 0;
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '|') {
            pipe_index = i;
            break;
        }
    }
    int tubeDesc[2];
    pid_t pid_fils;
    if (pipe (tubeDesc) == -1) {// [0]=lecture [1]=ecriture
        perror ("pipe");
        exit (1);
    }
    if ( (pid_fils = fork ( )) == -1 ){
        perror ("fork");
        exit (2);
    }
    if (pid_fils == 0) { /* fils */
        dup2(tubeDesc[1],STDOUT_FILENO);
        close (tubeDesc[1]);
        close (tubeDesc[0]);
        argv[pipe_index] = nullptr;
        if (execv(argv[1],argv+1) == -1) {
            perror ("execv");
            exit (3);
        }
    }
    else { /* père */
        dup2(tubeDesc[0],STDIN_FILENO);
        close (tubeDesc[0]);
        close (tubeDesc[1]);
        pipe_index++;
        if (execv(argv[pipe_index], argv + pipe_index) == -1) {
            perror ("execv");
            exit (3);
        }
    }
    return (EXIT_SUCCESS);
}