#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
//0666
#define FILE_MODE S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH
#define FIFO_NAME "ssu_fifofile"

int main(void) {
	char buf[BUFFER_SIZE];
	int fd;
	int length;

    //fifo 파일 생성
	mkfifo(FIFO_NAME, FILE_MODE);
	printf("waiting for readers...\n");
    
    //fifo 파일을 쓰기 전용으로 open -> 생산자
	if((fd = open(FIFO_NAME, O_WRONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", FIFO_NAME);
		exit(1);
	}
    
	printf("got a reader--type some stuff\n");
    //표준 입력으로 전달받은 데이터를 fifo 파일에 write하여 소비자에게 전달
	while(fgets(buf, BUFFER_SIZE, stdin), !feof(stdin)) {
		if((length = write(fd, buf, strlen(buf) - 1)) == -1) {
			fprintf(stderr, "write error\n");
			exit(1);
		}
		else
			printf("speak: wrote %d bytes\n", length);
	}

	exit(0);
}
