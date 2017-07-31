#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(void) {
	sigset_t set;

	//시그널 집합 초기화
	sigemptyset(&set);
	//SIGINT 시그널 추가
	sigaddset(&set, SIGINT);

	//SIGINT 시그널이 존재하므로 1 반환
	switch(sigismember(&set, SIGINT)) {
		case 1:
			printf("SIGINT is icluded.\n");
			break;
		case 0:
			printf("SIGINT is not included.\n");
			break;
		default:
			printf("failed to call sigismember()\n");
	}

	//SIGSYS 시그널이 존재하지 않으므로 0 반환
	switch(sigismember(&set, SIGSYS)) {
		case 1:
			printf("SIGSYS is included.\n");
			break;	
		case 0:
			printf("SIGSYS is not included.\n");
			break;
		default:
			printf("failed to call sigismember()\n");
	}

	exit(0);
}
