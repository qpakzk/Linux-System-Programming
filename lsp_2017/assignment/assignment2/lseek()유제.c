#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define CREAT_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

char buf1[] = "1234567890";
char buf2[] = "ABCDEFGHIJ";

int main(void) {
	char *fname = "ssu_hole.txt";
	char fd;

	//파일 생성
	if((fd = creat(fname, CREAT_MODE)) < 0) {
		fprintf(stderr, "creat error for %s\n", fname);
		exit(1);
	}	

	//buf1 배열에 저장된 문자열을 파일에 쓰기
	if(write(fd, buf1, 12) != 12) {
		fprintf(stderr, "buf1 write error\n");
		exit(1);
	}

	//파일 시작에서 15000만큼 떨어진 위치로 오프셋 지정
	if(lseek(fd, 15000, SEEK_SET) < 0) {
		fprintf(stderr, "lseek error\n");
		exit(1);
	}

	//buf2 배열에 저장된 문자열을 파일에 쓰기
	if(write(fd, buf2, 12) != 12) {
		fprintf(stderr, "buf2 write error\n");
	}

	exit(0);
}
