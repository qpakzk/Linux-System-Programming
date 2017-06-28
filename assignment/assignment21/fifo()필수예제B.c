#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define FIFO_NAME "ssu_fifofile"

int main(void) {
	char buf[BUFFER_SIZE];
	int fd;
	int length;

    //fifo 파일 생성
	mknod(FIFO_NAME, S_IFIFO | 0666, 0);
	printf("waiting for writers...\n");
	
    //fifo 파일 open
	if((fd = open(FIFO_NAME, O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", FIFO_NAME);
		exit(1);
	}	

	printf("got a writer\n");

	do {
        //fifo 파일을 이용하여 생산자로부터 데이터를 받아옴
		if((length = read(fd, buf, BUFFER_SIZE)) == -1) {
			fprintf(stderr, "read error\n");
			exit(1);
		}
		else {
			buf[length] = '\0';
			printf("tick: read %d bytes: \"%s\"\n", length, buf);
		}

	}
	while(length > 0);
			
	exit(0);	
}
