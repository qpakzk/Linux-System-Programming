#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	//자식 프로세스를 대신하여 새로운 프로세스 실행
	// /bin/echo를 실행시켜 전달한 인자 출력
	if(fork() == 0) {
		execl("/bin/echo", "echo", "this is", "message one", (char *)0);
		fprintf(stderr, "exec error\n");
		exit(1);
	}

	if(fork() == 0) {
		execl("/bin/echo", "echo", "this is", "message two", (char *)0);
		fprintf(stderr, "exec error\n");
		exit(1);
	}

	if(fork() == 0) {
		execl("/bin/echo", "echo", "this is", "message three", (char *)0);
		fprintf(stderr, "exec error\n");
		exit(1);
	}

	printf("Parent program ending\n");
	exit(0);	
}
