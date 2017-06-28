#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssu_employee.h"

int main(int argc, char *argv[]) {
	struct ssu_employee record;
	int fd;

    //shell에서 파일을 입력받음
	if(argc < 2) {
		fprintf(stderr, "usage : %s file\n", argv[0]);
		exit(1);
	}

    //쓰기 전용 모드로 파일을 open
    //파일이 없을 경우 0644 모드로 생성 후 쓰기 전용 모드로 파일을 open
	if((fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0640)) < 0) {
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}
	
	while(1) {
		printf("Enter employee name <SPACE> salary: ");
		scanf("%s", record.name);

		if(record.name[0] == '.')
			break;

		scanf("%d", &record.salary);
		record.pid = getpid();
        //record에 저장된 정보를 argv[1] 파일에 write하기
		write(fd, (char *)&record, sizeof(record));
	}

	close(fd);
	exit(0);
}
