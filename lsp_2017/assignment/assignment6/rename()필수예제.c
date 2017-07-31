#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int fd; 

	if(argc != 3) {
		fprintf(stderr, "usage: %s <oldname> <newname>\n", argv[0]);
		exit(1);
	}

	//파일을 읽기 전용으로 open
	if((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "first open error for %s\n", argv[1]);
		exit(1);
	}

	//argv[1] 파일을 argv[2]로 이름을 바꿈
	if(rename(argv[1], argv[2]) < 0) {
		fprintf(stderr, "rename error\n");
		exit(1);
	}

	//이름이 바뀌었기 때문에 에러
	if((fd = open(argv[1], O_RDONLY)) < 0)
		fprintf(stderr, "second open error for %s\n", argv[1]);
	else {
		fprintf(stderr, "it's very stange\n");
		exit(1);
	}

	//바뀐 이름이므로 open
	if((fd = open(argv[2], O_RDONLY)) < 0) {
		fprintf(stderr, "third open error for %s\n", argv[2]);
		exit(1);
	}

	printf("Everything is good!\n");
	exit(0);
}	
