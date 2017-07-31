#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if(argc != 3) {
		fprintf(stderr, "usage: %s <actualname> <symname>\n", argv[0]);
		exit(1);
	}	
	
	//argv[1]이 원본 파일인 심볼릭링크 파일 argv[2]를 만든다.	
	if(symlink(argv[1], argv[2]) < 0) {
		fprintf(stderr, "symlink error\n");
		exit(1);	
	}
	else
		printf("symlink: %s -> %s\n", argv[2], argv[1]);

	exit(0);
}
