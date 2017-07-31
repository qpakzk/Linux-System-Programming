#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void) {
	sigset_t pendingset;
	sigset_t sig_set;
	int count = 0;

	//sig_set을 1로 초기화
	sigfillset(&sig_set);
	//sig_set이 프로세스의 시그널 마스크가 됨
	sigprocmask(SIG_SETMASK, &sig_set, NULL);

	while(1) {
			printf("count: %d\n", count++);
		sleep(1);

		//유보 중인 시그널 집합을 찾음
		if(sigpending(&pendingset) == 0) {
			//SIGINT가 존재하는지 확인
			if(sigismember(&pendingset, SIGINT)) {
				printf("SIGINT 블록되어 대기 중. 무한 루프를 종료.\n");
				break;
			}
		}
	}

	exit(0);
}
