#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
	char *fname = "ssu_test.txt";
	int fd;

    //0666 모드로 파일 creat
	if((fd = creat(fname, 0666)) < 0) {
		printf("creat error for %s\n", fname);
		exit(1);
	}	

	printf("First printf is on the screen.\n");
    //fd file descripter의 값이 1에 복사되어
	dup2(fd, 1);
    //표준 출력이 fname 파일에 이루어졌다.
	printf("Second printf is in this file.\n");
	exit(0);
}
