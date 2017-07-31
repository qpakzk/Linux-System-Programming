#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssu_employee.h"

int main(int argc, char *argv[]) {
	struct ssu_employee record;
	int fd;
	int record_num;

    //shell을 통해 파일을 입력받음
	if(argc < 2) {
		fprintf(stderr, "Usage : %s file\n", argv[0]);
		exit(1);
	}
    
    //읽기 전용으로 입력받은 파일을 open
	if((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

	while(1) {
		printf("Enter record number : ");
		scanf("%d", &record_num);

		if(record_num < 0)
			break;
        
        //lseek을 이용하여 record 번호에 따라 오프셋 변경
		if(lseek(fd, (long)record_num * sizeof(record), 0) < 0) {
			fprintf(stderr, "lseek error\n");
			exit(1);		
		}

        //read한 정보를 record에 저장 후 해당 정보 출력
		if(read(fd, (char *)&record, sizeof(record)) > 0)
			printf("Employee : %s Salary : %d\n", record.name, record.salary);
		else
			printf("Record %d not found\n", record_num);
	}

	close(fd);
	exit(0);
}
