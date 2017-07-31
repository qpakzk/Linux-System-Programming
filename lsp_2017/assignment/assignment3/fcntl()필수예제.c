#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void) {
	char *filename = "ssu_test.txt";
	int fd1, fd2;
	int flag;

    //0644모드, 읽기 쓰기 모두 가능하도록 파일 open
    //O_APPEND에 의해 기존 파일에 이어서 내용쓰기 가능
	if((fd1 = open(filename, O_RDWR | O_APPEND, 0644)) < 0) {
		fprintf(stderr, "open error for %s\n", filename);
		exit(1);
	}

    //close-on-exec 비트 설정
	if(fcntl(fd1, F_SETFD, FD_CLOEXEC) == -1) {
		fprintf(stderr, "fcntl F_SETFD error\n");
		exit(1);
	}

    //fd1의 파일 상태 플래그 반환
	if((flag = fcntl(fd1, F_GETFL, 0)) == -1) {
		fprintf(stderr, "fcntl F_GETFL error\n");
		exit(1);
	}

    //O_APPEND 플래그 설정됨
	if(flag & O_APPEND)
		printf("fd1 : O_APPEND flag is set.\n");
	else
		printf("fd1 : O_APPEND flag is NOT set.\n");

	//fd1의 파일 디스크립터 플래그 반환
    if((flag = fcntl(fd1, F_GETFD, 0)) == -1) {
		fprintf(stderr, "fcntl F_GETFD error\n");
		exit(1);
	}

    //FD_CLOEXEC 플래그 설정됨
	if(flag & FD_CLOEXEC) 
		printf("fd1 : FD_CLOEXEC flag is set.\n");
	else
		printf("fd1 : FD_CLOEXEC flag is NOT set.\n");

    //파일 디스크립터 복사
	if((fd2 = fcntl(fd1, F_DUPFD, 0)) == -1) {
		fprintf(stderr, "fcntl F_DUPFD error\n");
		exit(1);
	}

    //fd2의 파일 상태 플래그 반환
	if((flag = fcntl(fd2, F_GETFL, 0)) == -1) {
		fprintf(stderr, "fcntl F_GETFL error\n");
		exit(1);
	}

    //O_APPEND 플래그 설정됨
	if(flag & O_APPEND)
		printf("fd2 : O_APPEND flag is set\n");
	else
		printf("fd2 : O_APPEND flag is NOT set\n");

    //fd2의 파일 상태 플래그 반환
	if((flag = fcntl(fd2, F_GETFD, 0)) == -1) {
		fprintf(stderr, "fcntl F_GETFD error\n");
		exit(1);
	}

    //FD_CLOEXEC 플래그 설정되지 않음
    //file descripter 복사 시
    //close-on-exec 기능은 복사되지 않음
	if(flag & FD_CLOEXEC)
		printf("fd2 : FD_CLOEXEC flag is set.\n");
	else
		printf("fd2 : FD_CLOEXEC flag is NOT set.\n");

	exit(0);
}
