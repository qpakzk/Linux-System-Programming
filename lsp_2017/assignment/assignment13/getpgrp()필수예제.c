#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	pid_t pgid;
	pid_t pid;

	pid = getpid();//프로세스 ID
	pgid = getpgrp();//현재 프로세스의 프로세스 그룹 ID
	printf("pid: %d, pgid: %d\n", pid, pgid);
	exit(0);
}	
