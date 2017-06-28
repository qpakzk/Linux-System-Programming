#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define S_MODE 0644
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	char buf[BUFFER_SIZE];
	int fd1, fd2;
	int length;

    //shell에서 file 두 개를 입력받음
	if(argc != 3) {
		fprintf(stderr, "Usage : %s filein fileout\n", argv[0]);
		exit(1);
	}

    //읽기 전용으로 argv[1] 파일 open
	if((fd1 = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

    //쓰기 전용으로 argv[1] 파일 open
	if((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_MODE)) < 0) {
		fprintf(stderr, "open error for %s\n", argv[2]);
		exit(1);
	}

    //argv[1] 파일의 내용을 read해서 argv[2] 파일에 write하기
	while((length = read(fd1, buf, BUFFER_SIZE)) > 0)
		write(fd2, buf, length);

	exit(0);
}
