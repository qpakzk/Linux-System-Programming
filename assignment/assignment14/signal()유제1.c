#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void ssu_signal_handler(int signo);

int main(void) {
	//시그널 핸들러 등록
	/*
	 * SIGINT : 사용자 터미널 DELETE or control + c를 통해서
	 * 인터럽트를 발생시키면 터미널 드라이버에 의해서 발생함.
	 */
	if(signal(SIGINT, ssu_signal_handler) == SIG_ERR) {
		fprintf(stderr, "cannot handle SIGINT\n");
		exit(EXIT_FAILURE);
	}	

	/*
	 * SIGTERM : kill(1) 명령이 기본적으로 전송하는 종료 시그널
	 */
	if(signal(SIGTERM, ssu_signal_handler) == SIG_ERR) {
		fprintf(stderr, "cannot handle SIGTERM\n");
		exit(EXIT_FAILURE);
	}	

	/*
  	 * SIGPROF : setitimer(2)로 설정된 프로파일링 구간 타이머가 만료될 때 발생함.
	 */	 
	if(signal(SIGPROF, ssu_signal_handler) == SIG_ERR) {
		fprintf(stderr, "cannot handle SIGTERM\n");
		exit(EXIT_FAILURE);
	}	

	/*
	 * SIGHUP : 연결이 끊어졌음을 제어 터미널 인터페이스가 검출했을 때
	 * 제어 터미널에 연관된 제어 프로세스로 이 시그널이 전송됨.
	 */
	if(signal(SIGHUP, ssu_signal_handler) == SIG_ERR) {
		fprintf(stderr, "cannot handle SIGHUP\n");
		exit(EXIT_FAILURE);
	}	

	while(1)
		pause();

	exit(0);
}


static void ssu_signal_handler(int signo) {
	if(signo == SIGINT)
		printf("caught SIGINT\n");
	else if(signo == SIGTERM)
		printf("caught SIGTERM\n");
	else {
		fprintf(stderr, "unexpected signal\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
