#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void ssu_signal_handler(int signo);
void ssu_timestamp(char *str);

int main(void) {
	struct sigaction sig_act;
	sigset_t blk_set;
	
	//blk_set을 1로 초기화
	sigfillset(&blk_set);
	//blk_set에서 SIGALRM 시그널 제거
	sigdelset(&blk_set, SIGALRM);
	//sig_act.sa_mask를 0으로 초기화
	sigemptyset(&sig_act.sa_mask);
	//sig_act 설정
	sig_act.sa_flags = 0;
	sig_act.sa_handler = ssu_signal_handler;
	//SIGALRM 시그널이 발생했을 때의 엑션 설정 
	sigaction(SIGALRM, &sig_act, NULL);
	ssu_timestamp("before sigsuspend()");
	alarm(5);

	//SIGALRM 시그널이 전달될 때까지 대기
	//SIGALRM 시그널이 전달되면 시그널 핸들러 함수 호출	
	sigsuspend(&blk_set);
	ssu_timestamp("after sigsuspend()");	
	exit(0);
}

//SIGALRM의 시그널 핸들러 함수
void ssu_signal_handler(int signo) {
	printf("in ssu_signal_handler() function\n");	
}

void ssu_timestamp(char *str) {
	time_t time_val;
	time(&time_val);
	printf("%s the time is %s\n", str, ctime(&time_val));
}
