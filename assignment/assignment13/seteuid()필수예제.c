#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void) {
	int fd, state;

	//실제 사용자 ID(1000)로 유효 사용자 ID를 변경 
	state = seteuid(1000);

	if(state < 0) {
		fprintf(stderr, "seteuid error\n");
		exit(1);
	}

	if((fd = open("ssu_test.txt", O_CREAT | O_RDWR, S_IRWXU)) < 0) {
		fprintf(stderr, "open error\n");
		exit(1);
	}

	close(fd);
	exit(0);
}	
