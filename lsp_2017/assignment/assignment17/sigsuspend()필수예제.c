#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void) {
	sigset_t old_set;	
	sigset_t sig_set;	

	//sig_set 0으로 초기화
	sigemptyset(&sig_set);
	//SIGINT 시그널을 sig_set에 추가
	sigaddset(&sig_set, SIGINT);
	//sig_set을 블록설정, old_set에 기존의 시그널 집합 저장
	sigprocmask(SIG_BLOCK, &sig_set, &old_set);
	//old_set에 있는 시그널이 발생하기 전까지 일시정지 
	sigsuspend(&old_set);
	exit(0);
}
