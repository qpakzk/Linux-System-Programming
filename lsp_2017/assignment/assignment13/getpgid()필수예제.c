#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	pid_t pgid;
	pid_t pid;
	
	if(argc < 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		exit(1);
	}

	pid = getpid();//프로세스 id
	pgid = getpgid(atoi(argv[1]));//프로세스의 그룹 id
	printf("pid: %d, pgid: %d\n", pid, pgid);
	exit(0);
}
