#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

static void ssu_func(int signo);
void ssu_print_mask(const char *str);

int main(void) {
	sigset_t new_mask, old_mask, wait_mask;

	ssu_print_mask("program start: ");

	//SIGINT 시그널에 대한 시그널 핸들러 함수로 ssu_func 등록
	if(signal(SIGINT, ssu_func) == SIG_ERR) {
		fprintf(stderr, "signal(SIGINT) error\n");
		exit(1);	
	}

	//wait_mask를 0으로 초기화
	sigemptyset(&wait_mask);
	//wait_mask에 SIGUSR1 시그널을 추가
	sigaddset(&wait_mask, SIGUSR1);
	//new_mask를 0으로 초기화
	sigemptyset(&new_mask);
	//new_mask에 SIGINT 시그널을 추가
	sigaddset(&new_mask, SIGINT);
	
	//new_mask를 블록설정, old_mask에 기존의 시그널 집합 저장
	if(sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
		fprintf(stderr, "SIG_BLOCK() error\n");
		exit(1);
	}

	ssu_print_mask("in critical region: ");

	//wait_mask로 마스크된 시그널들이 발생할 때까지 일시정지
	if(sigsuspend(&wait_mask) != -1) {
		fprintf(stderr, "sigsuspend() error\n");
		exit(1);
	}

	ssu_print_mask("after return from sigsuspend: ");

	//old_mask로 마스크
	if(sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
		fprintf(stderr, "SIG_SETMASK() error\n");
		exit(1);
	}

	ssu_print_mask("program exit: ");
	exit(0);
}

void ssu_print_mask(const char *str) {
	sigset_t sig_set;
	int err_num;

	err_num = errno;

	//시그널 마스크가 안변함
	if(sigprocmask(0, NULL, &sig_set) < 0) {
		fprintf(stderr, "sigprocmask() error\n");
		exit(1);
	}

	printf("%s", str);

	//sig_set에 존재하는 시그널 찾기
	if(sigismember(&sig_set, SIGINT))
		printf("SIGINT ");

	if(sigismember(&sig_set, SIGQUIT))
		printf("SIGQUIT ");

	if(sigismember(&sig_set, SIGUSR1))
		printf("SIGUSR1 ");

	if(sigismember(&sig_set, SIGALRM))
		printf("SIGALRM ");

	printf("\n");
	errno = err_num;
}

//SIGINT의 시그널 핸들러 함수
static void ssu_func(int signo) {
	ssu_print_mask("\nin ssu_func: ");
}
