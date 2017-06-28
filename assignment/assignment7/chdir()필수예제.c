#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	// /etc 디렉터리로 이동
	if(chdir("/etc") < 0) {
		fprintf(stderr, "chdir error\n");
		exit(1);
	}

	printf("chdir to /etc succeeded.\n");
	exit(0);
}	
