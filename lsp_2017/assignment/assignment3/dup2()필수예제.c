#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(void) {
	char buf[BUFFER_SIZE];
	char *fname = "ssu_test.txt";
	int fd;
	int length;

    //0644 모드, 읽기 전용으로 파일 open
	if((fd = open(fname, O_RDONLY, 0644)) < 0) {
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

    //1번 file descripter를 4번으로 복사
	if(dup2(1, 4) != 4) {
		fprintf(stderr, "dup2 call failed\n");
		exit(1);
	}
    
	while(1) {
        //파일 내용을 buf에 read해오기
		length = read(fd, buf, BUFFER_SIZE);

		if(length <= 0)
			break;

        //4번 file descripter는 1번을 복사해 왔으므로
        //buf에 저장된 데이터를 표준 출력한다.
		write(4, buf, length);
	}

	exit(0);
}
