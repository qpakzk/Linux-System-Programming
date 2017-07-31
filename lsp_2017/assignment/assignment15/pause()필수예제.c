#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_alarm(int signo);

int main(void) {
	printf("Alarm Setting\n");
	//SIGALRM에 대한 ssu_alarm 시그널 핸들러 등록
	signal(SIGALRM, ssu_alarm);
	//2초 후에 SIGALRM 시그널을 발생
	alarm(2);

	while(1) {
		printf("done\n");
		//SIGALRM 시그널을 받을 때까지 
		//프로세스의 수행을 중지
		pause();
		alarm(2);
	}	
	exit(0);
}

void ssu_alarm(int signo) {
	printf("alarm..!!!\n");
}
