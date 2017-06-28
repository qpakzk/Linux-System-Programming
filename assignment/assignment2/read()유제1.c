#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFFER_SIZE 1024

int main(void) {
	char buf[BUFFER_SIZE];
	char *fname = "ssu_test.txt";
	int count;
	int fd1, fd2;

	//읽기 전용으로 파일 open
	fd1 = open(fname, O_RDONLY, 0644);
	fd2 = open(fname, O_RDONLY, 0644);

	//예외 체크
	if(fd1 < 0 || fd2 < 0) {
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}	
	

	//buf로 25바이트만큼 읽어오기
	count = read(fd1, buf, 25);
	buf[count] = 0;
	printf("fd1's first printf : %s\n", buf);
	//파일의 현재 위치에서 1만큼 더한 위치로 이동
	lseek(fd1, 1, SEEK_CUR);
	//buf로 24바이트만큼 읽어오기
	count = read(fd1, buf, 24);
	buf[count] = 0;
	printf("fd1's second printf : %s\n", buf);
	//buf로 25바이트만큼 읽어오기
	count = read(fd2, buf, 25);
	buf[count] = 0;
	printf("fd2's first printf : %s\n", buf);
	//현재 위치에서 1만큼 더한 위치로 이동
	lseek(fd2, 1, SEEK_CUR);
	//buf로 24바이트만큼 읽어오기
	count = read(fd2, buf, 24);
	buf[count] = 0;
	printf("fd2's second printf : %s\n", buf);

	exit(0);
}
