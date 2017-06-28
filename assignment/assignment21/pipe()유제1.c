#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(void) {
	char buf[BUFFER_SIZE];
	int pid;
	int pipe_fd[2];

    //파이프의 입출력 fd를 pipe_fd에 저장
	pipe(pipe_fd);

    //자식 프로세스 생성
	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}	
	else if(pid == 0) {//자식 프로세스만 실행
		printf(" CHILD: writing to the pipe\n");
		write(pipe_fd[1], "OSLAB", 6);//파이프에 OSLAB을 write
		printf(" CHILD: exiting\n");
		exit(0);
	}
	else {//부모 프로세스만 실행
		printf("PARENT: reading from pipe\n");
        //파이프에 저장된 데이터를 buf로 read
		read(pipe_fd[0], buf, 6);
		printf("PARENT: read \"%s \"\n", buf);
		wait(NULL);
	}

	exit(0);
}	
