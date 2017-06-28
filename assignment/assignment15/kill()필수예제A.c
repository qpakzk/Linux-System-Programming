#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//kill()을 수행하는 프로세스
int main(int argc, char *argv[]) {
	if(argc != 2) {
		fprintf(stderr, "usage: %s [Process ID]\n", argv[0]);
		exit(1);
	} 
	else
		kill(atoi(argv[1]), SIGKILL);

	exit(0);
}
