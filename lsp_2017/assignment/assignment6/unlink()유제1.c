#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
	char buf[64];
	char *fname = "ssu_tempfile";
	int fd;
	int length;
	//파일을 0600 모드로 생성하고 읽기 쓰기 전용으로 open한다.
	if((fd = open(fname, O_RDWR | O_CREAT | O_TRUNC, 0600)) < 0) {
		fprintf(stderr, "first open error for %s\n", fname);
		exit(1);
	}

	//파일은 unlink한다.
	if(unlink(fname) < 0) {
		fprintf(stderr, "unlink error for %s\n", fname);
		exit(1);
	}

	//파일에write
	if(write(fd, "How are you?", 12) != 12) {
		fprintf(stderr, "write error\n");
		exit(1);
	}

	//파일 오프셋을 파일의 처음으로 옮김
	lseek(fd, 0, 0);

	//buf에 파일 내용을 read
	if((length = read(fd, buf, sizeof(buf))) < 0) {
		fprintf(stderr, "buf read error\n");
		exit(1);
	}
	else
		buf[length] = 0;

	printf("%s\n", buf);
	close(fd);

	//파일이 unlink()로 삭제되어 open 에러
	if((fd = open(fname, O_RDWR)) < 0) {
		fprintf(stderr, "second open error for %s\n", fname);
		exit(1);
	}
	else 
		close(fd);

	exit(0);
}
