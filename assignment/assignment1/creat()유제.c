#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
	char *fname = "ssu_test.txt";
	int fd;

	//오류 체크 : 오류 시 -1 반환
	if((fd = creat(fname, 0666)) < 0) {//666모드로 파일 생성
		fprintf(stderr, "creat error for %s\n", fname);
		exit(1);
	}
	else {
		close(fd);//파일을 close한다.
		fd = open(fname, O_RDWR);//파일을 읽기 쓰기가 모두 가능하도록 open한다. 
		printf("Succeeded!\n<%s> is new readable and writable\n", fname);
	}

	exit(0);
}
