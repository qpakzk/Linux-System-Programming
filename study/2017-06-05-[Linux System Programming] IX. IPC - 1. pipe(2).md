---
layout: post
title: "[Linux System Programming] IX. IPC - 1. pipe(2)"
tags: Linux_System_Programming Spring_2017
---

# UNIX 통신

- 단방향 통신 : pipe
- 양방향 통신 : message passing, semaphore, shared memory

# pipe vs redirection

- pipe : [process1] \| [process2]
  - process1의 standard input을 process2의 standard output으로 연결
- redirection
  - [process] > [file] : process의 standard output 결과를 file에 출력
  - [process] < [file] : file의 내용을 process의 standard input으로 입력

# pipe(2)

```c
#include <unistd.h>
int pipe(int filedes[2]);

//반환값 : 성공 시 0, 실패 시 -1을 반환하고 errno가 설정됨
```

- 인자
  - filedes[0] : 읽기를 위한 파이프의 파일 디스크립터
  - filedes[1] : 쓰기를 위한 파이프의 파일 디스크립터
- 자식 프로세스와 부모 프로세스 간 통신을 지원
- pipe
  - UNIX/LINUX 시스템에서 제공하는 단방향 IPC
  - 한 프로세스의 출력을 다른 프로세스의 입력으로 연결
  - 프로세스가 읽기와 쓰기 통신을 모두 해야한다면 파이프 두 개 필요

# ssu_pipe.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void) {
	int pipe_fd[2];
	char buf[BUFFER_SIZE];

	if(pipe(pipe_fd) < 0) {
		fprintf(stderr, "pipe error\n");
		exit(1);
	}

	printf("writing to file descripter #%d\n", pipe_fd[1]);
	write(pipe_fd[1], "OSLAB", 6);
	printf("reading from file descripter #%d\n", pipe_fd[0]);
	read(pipe_fd[0], buf, 6);
	printf("read \"%s \"\n", buf);
	exit(0);
}
```

![alt text]({{ site.baseurl }}/image/lsp/IX/ssu_pipe.png)

# ssu_pipe_1.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(void) {
	int pipe_fd[2];
	pid_t pid;
	char buf[BUFFER_SIZE];

	if(pipe(pipe_fd) < 0) {
		fprintf(stderr, "pipe error\n");
		exit(1);
	}

	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if(pid == 0) {
		printf(" CHILD: writing to pipe\n");
		write(pipe_fd[1], "OSLAB", 6);
		printf(" CHILD: exiting\n");
		exit(0);
	}
	else {
		wait(NULL);
		printf("PARENT: reading from pipe\n");
		read(pipe_fd[0], buf, 6);
		printf("PARENT: read \"%s \"\n", buf);
	}
	exit(0);
}
```

![alt text]({{ site.baseurl }}/image/lsp/IX/ssu_pipe_1.png)

# ssu_pipe_2.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(void) {
	int pipe_fd[2];
	pid_t pid;
	char buf[BUFFER_SIZE];

	if(pipe(pipe_fd) < 0) {
		fprintf(stderr, "pipe error\n");
		exit(1);
	}

	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if(pid == 0) {
		close(pipe_fd[0]);
		while(1) {
			memset(buf, 0x00, BUFFER_SIZE);
			sprintf(buf, "Hello Mother Process. My name is %d\n", getpid());
			write(pipe_fd[1], buf, strlen(buf));
			sleep(1);
		}
	}
	else {
		close(pipe_fd[1]);
		while(1) {
			memset(buf, 0x00, BUFFER_SIZE);
			read(pipe_fd[0], buf, BUFFER_SIZE);
			fprintf(stderr, "%s", buf);
		}
	}
	exit(0);
}
```

![alt text]({{ site.baseurl }}/image/lsp/IX/ssu_pipe_2.png)
