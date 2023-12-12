#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main () {
	const int N = 3;

	std::cout << "main pid=" << getpid() << std::endl;
	int nb =0;
	for (int i=1, j=N; i<=N && j==N; i++ ) {
		if(fork() == 0){
			nb=0;
			std::cout << " i:j " << i << ":" << j << std::endl;
		}else{
			nb++;
			break;
		}
		for (int k=1; k<=i && j==N ; k++) {
			if ( fork() == 0) {
				nb=0;
				j=0;
				std::cout << " k:j " << k << ":" << j << std::endl;
			}else{
				nb++;

			}
		}
	}
	for (int i=0;i<nb;i++){
		int status;
		int pid =wait(&status);
		std::cout<<"fin de "<< pid <<"  retourne "<< WEXITSTATUS(status)<<std::endl;
		wait(nullptr);

	}
	return 0;
}
