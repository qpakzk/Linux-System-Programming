#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	struct stat statbuf;
	
	//shell을 통해서 파일을 입력받음
	if(argc != 2) {
		fprintf(stderr, "usage : %s <file>\n", argv[0]);
		exit(1);
	}

	//stat()를 통해 statbuf에 파일 정보 저장
	if((stat(argv[1], &statbuf)) < 0) {
		fprintf(stderr, "stat error\n");
		exit(1);
	}

	//macOS에서 warning 뜸
	//printf("%s is %ld bytes\n", argv[1], statbuf.st_size);
	//argv[1]의 크기 출력
	printf("%s is %lld bytes\n", argv[1], statbuf.st_size);
	exit(0);
}
