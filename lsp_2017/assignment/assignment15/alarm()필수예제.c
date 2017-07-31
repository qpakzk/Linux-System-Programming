#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo);

int count = 0;

int main(void) {
	//SIGALRM 시그널에 대한 ssu_signal_handler 등록
	signal(SIGALRM, ssu_signal_handler);
	//1초 후 알람 실행
	alarm(1);

	while(1);

	exit(0);
}

void ssu_signal_handler(int signo) {
	printf("alarm %d\n", count++);
	//1초 후 알람 실행
	alarm(1);
}
