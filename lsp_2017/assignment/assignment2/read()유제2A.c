#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
	char c;
	int fd;
    //읽기 전용으로 파일 open
	if((fd = open("ssu_test.txt", O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", "ssu_test.txt");
		exit(1);
	}

	while(1) {
        //c로 문자를 하나씩 read하기
		if(read(fd, &c, 1) > 0)
			putchar(c);
		else
			break;
	}

	exit(0);
}
