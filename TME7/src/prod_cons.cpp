#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/mman.h>


#define NBPROD 2
#define NBCONS 3

using namespace std;
using namespace pr;

void producteur (Stack<char> * stack) {
    char c ;
    while (cin.get(c)) {
        stack->push(c);
    }
}

void consomateur (Stack<char> * stack) {
    while (true) {
        char c = stack->pop();
        cout << c <<flush;
    }
}

int main () {
    void * space = mmap(0,sizeof(Stack<char>),PROT_READ|PROT_WRITE,MAP_ANONYMOUS | MAP_SHARED,-1,0);
    if(space == MAP_FAILED){
        perror("MMAP FAILED");
        exit(1);
    }
    Stack<char> * s = new Stack<char>();

    for (int i = 0; i < NBPROD; i++){
        if (fork() == 0) {
            producteur(s);
            cout<<"Prod : "<<i<<endl;
            return 0;
        }
    }

    for (int i = 0; i < NBCONS; i++){
        if (fork()==0) {
            consomateur(s);
            cout<<"Consom : "<<i<<endl;
            return 0;
        }
    }

    for (int i = 0; i < NBPROD+NBCONS; i++){
        wait(0);
    }

    s->~Stack();
    return 0;
}

