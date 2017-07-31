#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssu_employee.h"

#define DUMMY 0

int main(int argc, char *argv[]) {
	struct ssu_employee record;
	int fd;
	int flags;
	int length;
	int pid;

    //shell을 통해 파일을 입력받는다.
	if(argc < 2) {
		fprintf(stderr, "Usage : %s file\n", argv[0]);
		exit(1);
	}	

    //읽기, 쓰기가 가능하도록 파일을 open
	if((fd = open(argv[1], O_RDWR)) < 0) {
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

    //fd의 파일 상태 플래그 반환
	if((flags = fcntl(fd, F_GETFL, DUMMY)) == -1) {
		fprintf(stderr, "fcntl F_GETFL error\n");
		exit(1);
	}

    //flags에 O_APPEND 추가
	flags |= O_APPEND;

    //fd의 파일 상태 플래그를 세 번째 인자로 설정
	if(fcntl(fd, F_SETFL, flags) == -1) {
		fprintf(stderr, "fcntl F_SETFL error\n");
		exit(1);
	}
    
    //process ID
	pid = getpid();

    //argv[1]에 정보 추가
	while(1) {
		printf("Enter employee name : ");
		scanf("%s", record.name);

		if(record.name[0] == '.')
			break;

		printf("Enter employee salary : ");
		scanf("%d", &record.salary);
		record.pid = pid;
		length = sizeof(record);
	
		if(write(fd, (char *)&record, length) != length) {
			fprintf(stderr, "record write error\n");
			exit(1);
		}

	}

	close(fd);
	exit(0);	
}
