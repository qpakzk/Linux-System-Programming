#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>//oflag 인자 정의

int main(void) {
	char *fname = "ssu_test.txt";
	int fd;

	//오류 체크 : 오류 시 -1 반환
	if((fd = open(fname, O_RDONLY)) < 0) {//읽기 전용으로 파일을 open한다.
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}
	else//file descripter의 0, 1, 2는 각각 표준 입력, 표준 출력, 표준 오류로 예약되어 있으므로 이 경우 3을 반환한다.
		printf("Success!\nFilename : %s\nDescripter : %d\n", fname, fd);

	exit(0);
}
