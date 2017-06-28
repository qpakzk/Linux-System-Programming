#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
	char character;
	int fd;
	int line_count = 0;
    
    //읽기 전용으로 파일 open
	if((fd = open("ssu_test.txt", O_RDONLY))  < 0) {
		fprintf(stderr, "open error for %s\n", "ssu_test.txt");
		exit(1);
	}

	while(1) {
        //character로 문자를 하나씩 read하기
		if(read(fd, &character, 1) > 0) {
			//line 개수 세기
            if(character == '\n')
                line_count++;
		}		
		else
			break;
	}
    
	printf("Total line : %d\n", line_count);
	exit(0);
}
