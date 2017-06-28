#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void ssu_signal_handler1(int signo);
void ssu_signal_handler2(int signo);

int main(void) {
	//SIGINT에 대한 ssu_signal_handler1 등록
	if(signal(SIGINT, ssu_signal_handler1) == SIG_ERR) {
		fprintf(stderr, "cannot handle SIGINT\n");
		exit(EXIT_FAILURE);
	}

	//SIGUSR1에 대한 ssu_signal_handler2 등록
	if(signal(SIGUSR1, ssu_signal_handler2) == SIG_ERR) {
		fprintf(stderr, "cannot handle SIGUSER1\n");
	   exit(EXIT_FAILURE);	
	}

	//SIGINT, SIGUSR1를 자기 자신에게 전송한다.
	raise(SIGINT);
	raise(SIGUSR1);
	printf("main return\n");
	exit(0);
}

void ssu_signal_handler1(int signo) {
	printf("SIGINT 시그널 발셍\n");
}

void ssu_signal_handler2(int signo) {
	printf("SIGUSR1 시그널 발생\n");
}
