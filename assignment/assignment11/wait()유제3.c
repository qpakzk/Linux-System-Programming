#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
	pid_t child1, child2;
	int pid, status;

	if((child1 = fork()) == 0)//자식 프로세스 실행
		execlp("date", "date", (char *)0);//새로운 프로세스 실행

	if((child2 = fork()) == 0)//자식 프로세스 실행
		execlp("who", "who", (char *)0);//새로운 프로세스 실행

	printf("parent: waiting for children\n");

	while((pid = wait(&status)) != -1) {
		if(child1 == pid)//첫 번째 자식 프로세스 exit()인자 출력
			printf("parent: first child: %d\n", (status >> 8));
		else if(child2 == pid)//두 번째 자식 프로세스 exit()인자 출력
			printf("parent: second child: %d\n", (status >> 8));
	}

	printf("parent: all children terminated\n");
	exit(0);
}
