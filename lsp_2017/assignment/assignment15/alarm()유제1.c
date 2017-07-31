#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define LINE_MAX 2048

static void ssu_alarm(int signo);

int main(void) {
	char buf[LINE_MAX];
	int n;

	//SIGALRM에 대한 ssu_alarm 등록
	if(signal(SIGALRM, ssu_alarm) == SIG_ERR) {
		fprintf(stderr, "SIGALRM error\n");
		exit(1);
	}

	//10초 후 알람 실행
	alarm(10);

	//buf에 표준입력
	if((n = read(STDIN_FILENO, buf, LINE_MAX)) < 0) {
		fprintf(stderr, "read() error\n");
		exit(1);
	}

	//실행 안 함, alarm()이 여러 개일 시 마지막 것만 실행 
	alarm(0);
	//buf에 저장된 데이터를 표준출력으로 출력
	write(STDOUT_FILENO, buf, n);
	exit(0);
}

static void ssu_alarm(int signo) {
	printf("ssu_alarm() called!\n");
}
