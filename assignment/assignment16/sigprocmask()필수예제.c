#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void) {
	sigset_t sig_set;
	int count;

	//sig_set을 0으로 초기화
	sigemptyset(&sig_set);
	//SIGINT 시그널을 sig_set에 추가
	sigaddset(&sig_set, SIGINT);
	//SIGINT를 블록할 시그널로 만듦
	sigprocmask(SIG_BLOCK, &sig_set, NULL);

	for(count = 3; 0 < count; count--) {
		printf("count %d\n", count);
		sleep(1);
	}

	printf("Ctrl-C에 대한 블록을 해제\n");
	//SIGINT에 대한 블록 해제 -> SIGINT의 디폴트 핸들러 수행가능
	sigprocmask(SIG_UNBLOCK, &sig_set, NULL);
	printf("count중 Ctrl-C입력하면 이 문장은 출력되지 않음.\n");

	while(1);

	exit(0);
}
