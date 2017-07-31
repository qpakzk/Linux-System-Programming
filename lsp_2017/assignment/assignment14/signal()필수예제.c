#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo);

void (*ssu_func)(int);

int main(void) {
	//SIGINT의 시그널 핸들러로 ssu_signal_handler() 등록
	//ssu_func에 이전 시그널 핸들러 등록
	ssu_func = signal(SIGINT, ssu_signal_handler);

	while(1) {
		printf("process running...\n");
		sleep(1);
	}
		
	exit(0);
}

void ssu_signal_handler(int signo) {
	printf("SIGINT 시그널 발생.\n");
	printf("SIGINT를 SIGDEL로 재설정함.\n");
	signal(SIGINT, ssu_func);//이전 시그널 핸들러인 SIGDFL로 재설정
}
