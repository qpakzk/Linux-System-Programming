#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define TABLE_SIZE 128
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	static struct {
		long offset;
		int length;
	} table[TABLE_SIZE];
	char buf[BUFFER_SIZE];
	long offset;
	int entry;
	int i;
	int length;
	int fd;

    //shell에서 파일을 인자로 입력받음
	if(argc < 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		exit(1);
	}		

    //입력 받은 파일을 읽기 전용으로 open
	if((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

	entry = 0;
	offset = 0;
    //buf로 BUFFER_SIZE만큼 read하기
	while((length = read(fd, buf, BUFFER_SIZE)) > 0) {
		for(i = 0; i < length; i++) {
			table[entry].length++;
			offset++;

			if(buf[i] == '\n')
				table[++entry].offset = offset;
		}
	}

//gcc -o ssu_read_3 read()유제3.c -D DEBUG 시 실행
#ifdef DEBUG
	for(i = 0; i < entry; i++)
		printf("%d : %ld, %d\n", i + 1, table[i].offset, table[i].length);
#endif

	while(1) {
		printf("Enter line number : ");
		scanf("%d", &length);
        
        //while문 나올 때
		if(--length < 0) 
			break;

        //파일 처음에서 table[length].offset에 저장된 오프셋만큼 이동
		lseek(fd, table[length].offset, 0);

        //buf로 table[length].length 만큼 read하기
		if(read(fd, buf, table[length].length) <= 0)
			continue;
        
		buf[table[length].length] = '\0';
		printf("%s", buf);
	}

	close(fd);
	exit(0);
}
