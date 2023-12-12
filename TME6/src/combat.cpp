#include <csignal>
#include <iostream>
#include <sys/signal.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <string>

//Variable Global
int PV = 4;
bool is_child = false;
std::string name = "Vador";

void randsleep() {
    int r = rand();
    double ratio = (double)r / (double) RAND_MAX;
    struct timespec tosleep;
    tosleep.tv_sec =0;
    // 300 millions de ns = 0.3 secondes
    tosleep.tv_nsec = 300000000 + ratio*700000000;
    struct timespec remain;
    while ( nanosleep(&tosleep, &remain) != 0) {
        tosleep = remain;
    }
}


void attaque(pid_t adversaire){
    signal(SIGINT,[](int sig){
        PV--;
        std::cout << name <<" a "<< PV <<" PV "<<std::endl;
        if (PV ==0){
            std::cout << name <<" est mort"<<std::endl;
            exit(1);
        }
    });
    waitpid(adversaire, nullptr, WNOHANG);
    int res = kill(adversaire, SIGINT);
    if (res < 0) {
        exit(0);
    }
    randsleep();
}

void defense(){
    if (is_child) {//Q5
        struct sigaction action;
        action.sa_handler = [](int sig) {
            std::cout << "coup pare par Luke" << std::endl;
        };
        sigaction(SIGINT, &action, NULL);

        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGINT);
        sigprocmask(SIG_BLOCK, &mask, NULL);

        randsleep();

        sigset_t set;
        sigfillset(&set);
        sigdelset(&set, SIGINT);
        sigsuspend(&set);
    }
    signal (SIGINT, SIG_IGN);
    randsleep();
}

void combat(pid_t adversaire) {
    while(true) {
        defense();
        attaque(adversaire);
    }
}


int main() {
    pid_t fils_pid = fork();

    if (fils_pid < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    srand(getpid()); //Q5

    if (fils_pid == 0) {
        name = "Luke";
        is_child=true;
        combat(getppid());
    }else{
        combat(fils_pid);
    }
    return 0;
}