#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

void ssu_signal_handler(int signo);

jmp_buf jump_buffer;

int main(void) {
	//SIGINT 시그널에 대한 시그널 핸들러 함수 등록
	signal(SIGINT, ssu_signal_handler);

	while(1) {
		if(setjmp(jump_buffer) == 0) {
			//setjmp()가 직접 호출된 경우
			printf("Hit Ctrl-c at anytime ... \n");
			pause();
		}
	}

	exit(0);
}

void ssu_signal_handler(int signo) {
	char character;

	//signo에 저장된 시그널을 무시
	signal(signo, SIG_IGN);
	printf("Did you hit Ctrl-c?\n""Do you really want to quit? [y/n] ");
	character = getchar();

	if(character == 'y' || character == 'Y')
		exit(0);
	else {
		//SIGINT 시그널에 대한 시그널 핸들러 함수 등록
		signal(SIGINT, ssu_signal_handler);
		//setjmp()로 이동
		longjmp(jump_buffer, 1);
	}
}
