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
	else {//file descripter의 0, 1, 2는 각각 표준 입력, 표준 출력, 표준 오류로 예약되어 있으므로 이 경우 3을 반환한다.
		printf("Success!\nFilename : %s\nDescripter : %d\n", fname, fd);
		close(fd);//파일을 close한다.
	}

	exit(0);
}
