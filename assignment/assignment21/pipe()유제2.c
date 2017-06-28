#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(void) {
	char buf[BUFFER_SIZE];
	int pid;
	int pipe_fd[2];

    //pipe_fd에 pipe fd 저장
	if(pipe(pipe_fd) < 0) {
		fprintf(stderr, "pipe error\n");
		exit(1);
	}

    //자식 프로세스 생성
	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if(pid == 0) {//자식 프로세스에세만 실행
		close(pipe_fd[0]);//입력 fd를 close
		
		while(1) {
			memset(buf, 0x00, BUFFER_SIZE);
            sprintf(buf, "Hello Mother Process. My name is %d\n", getpid());
            //buf의 데이터를 pipe에 write
			write(pipe_fd[1], buf, strlen(buf));
			sleep(1);
		}	
	}
	else {//부모 프로세스에서만 실행
		close(pipe_fd[1]);//출력 fd를 close

		while(1) {
			memset(buf, 0x00, BUFFER_SIZE);
            //pipe에 저장된 데이터를 buf로 read
			read(pipe_fd[0], buf, BUFFER_SIZE);
			fprintf(stderr, "%s", buf);
		}
	}
	
	exit(0);
}
