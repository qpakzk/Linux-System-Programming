#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define EXIT_CODE 1

int main(void) {
	pid_t pid;
	int ret_val, status;

	if((pid = fork()) == 0) {//자식 프로세스 생성
		printf("child: pid = %d ppid = %d exit_code = %d\n",
				getpid(), getppid(), EXIT_CODE);
		exit(EXIT_CODE);
	}

	printf("parent: waiting for child = %d\n", pid);
	//자식 프로세스 종료 상태 회수
	ret_val = wait(&status);
	//종료된 자식 프로세스 출력
	printf("parent: return value = %d, ", ret_val);
	printf(" child's status = %x", status);
	//status에 저장된 exit()의 인자 출력
	printf(" and shifted = %x\n", (status >> 8));
	exit(0);
}
