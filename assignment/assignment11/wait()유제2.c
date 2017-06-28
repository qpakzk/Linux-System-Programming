#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
	if(fork() == 0)//자식 프로세스 실행
		execl("/bin/echo", "echo", "this is", "message one", (char *)0);//새로운 프로세스 실행

	if(fork() == 0)//자식 프로세스 실행
		execl("/bin/echo", "echo", "this is", "message two", (char *)0);//새로운 프로세스 실행

	printf("parent: waiting for children\n");

	while(wait((int *)0) != -1);//자식 프로세스가 끝날 때까지 부모 프로세스 대기

	printf("parent: all children terminated\n");
	exit(0);	
}
