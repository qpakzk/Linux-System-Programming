#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(void) {
	char buf[BUFFER_SIZE];
	int pipe_fd[2];

    //pip_fd 배열에 pipe의 입/출력 fd 저장
	if(pipe(pipe_fd) == -1) {
		fprintf(stderr, "pipe error\n");
		exit(1);
	}

	printf("writing to file descripter #%d\n", pipe_fd[1]);
	write(pipe_fd[1], "OSLAB", 6);//pipe에 OSLAB을 write
	printf("reading from file descripter #%d\n", pipe_fd[0]);
	read(pipe_fd[0], buf, 6);//pipe에 저장된 OSLAB을 buf로 read
	printf("read \"%s \" \n", buf);
	exit(0);
}
