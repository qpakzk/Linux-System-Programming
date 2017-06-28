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

    //0644모드, 읽기 전용으로 파일을 open
	if((fd1 = open(fname, O_RDONLY, 0644)) < 0) {
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

    //file descripter 복사
	fd2 = dup(fd1);
    //buf에 파일 내용을 read해오기
	count = read(fd1, buf, 12);
	buf[count] = 0;
	printf("fd1's printf : %s\n", buf);
    //현재 오프셋에서 1만큼 오프셋 이동
	lseek(fd1, 1, SEEK_CUR);
    //복사한 file descripter를 이용하여 buf에 파일 내용을 read해오기
	count = read(fd2, buf, 12);
	buf[count] = 0;
	printf("fd2's printf : %s\n", buf);
	exit(0);
}
