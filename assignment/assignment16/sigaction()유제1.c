#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_check_pending(int signo, char *signame);
void ssu_signal_handler(int signo);

int main(void) {
	struct sigaction sig_act;
	sigset_t sig_set;

	//sig_act 구조체를 설정
	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = 0;
	sig_act.sa_handler = ssu_signal_handler;

	//SIGUSR1 시그널이 발생했을 때의 엑션에 대한 정보 전달
	if(sigaction(SIGUSR1, &sig_act, NULL) != 0) {
		fprintf(stderr, "sigaction() error\n");
		exit(1);
	}
	else {
		//sig_set을 0으로 초기화
		sigemptyset(&sig_set);
		//sig_set에 SIGUSR1 시그널 추가
		sigaddset(&sig_set, SIGUSR1);

		//sig_set에 있는 시그널을 마스크함
		if(sigprocmask(SIG_SETMASK, &sig_set, NULL) != 0) {
			fprintf(stderr, "sigprocmask() error\n");
			exit(1);
		}
		else {
			printf("SIGUSR1 signals are now blocked\n");
			kill(getpid(), SIGUSR1);//시그널이 마스크됨
			printf("after kill()\n");
			ssu_check_pending(SIGUSR1, "SIGUSR1");//유보된 시그널이 존재
			sigemptyset(&sig_set);//sig_set을 0으로 초기화
			//sig_set에 시그널이 없어 마스크되는 시그널이 없음
			sigprocmask(SIG_SETMASK, &sig_set, NULL);
			printf("SIGUSR1 signals are no longer blocked\n");
			ssu_check_pending(SIGUSR1, "SIGUSR1");//유보된 시그널이 없음
		}
	}
	exit(0);
}

void ssu_check_pending(int signo, char *signame) {
	sigset_t sig_set;

	//유보 중인 시그널이 있는지 확인
	if(sigpending(&sig_set) != 0)
		printf("sigpending() error\n");
	else if(sigismember(&sig_set, signo))//유보된 시그널이 있다면
		printf("a %s signal is pending\n", signame);
	else//유보된 시그널이 없다면
		printf("%s signals are not pending\n", signame);
}

void ssu_signal_handler(int signo) {
	printf("in ssu_signal_handler function\n");
}
