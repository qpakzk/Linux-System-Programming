#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo) {
	printf("ssu_signal_handler control\n");
}

int main(void) {
	struct sigaction sig_act;
	sigset_t sig_set;

	//sig_act.sa_mask를 0으로 초기화
	sigemptyset(&sig_act.sa_mask);
	//sig_act 구조체 값 설정
	sig_act.sa_flags = 0;
	sig_act.sa_handler = ssu_signal_handler;

	//SIGUSR1 시그널이 발생했을 때의 엑션에 대한 정보 전달
	sigaction(SIGUSR1, &sig_act, NULL);
	printf("before first kill()\n");
	//현재 프로세스에 SIGUSR1 시그널을 발생시킴
	kill(getpid(), SIGUSR1);

	//sig_set을 0으로 초기화
	sigemptyset(&sig_set);
	//SIGUSR1 시그널을 sig_set에 추가
	sigaddset(&sig_set, SIGUSR1);

	//sig_set에 있는 시그널을 마스크함
	sigprocmask(SIG_SETMASK, &sig_set, NULL);
	printf("before second kill()\n");
	kill(getpid(), SIGUSR1);//마스크가 되어 시그널 핸들러가 수행되지 않음
	printf("after second kill()\n");
	exit(0);
}
