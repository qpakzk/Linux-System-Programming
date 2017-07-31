#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if(argc != 3) {
		fprintf(stderr, "usage: %s <oldname> <newname>\n", argv[0]);
		exit(1);
	}

	//argv[2] 파일이 argv[1] 파일을 링크한다.
	if(link(argv[1], argv[2]) < 0) {
		fprintf(stderr, "link error\n");
		exit(1);
	}
	else
		printf("step1 passed.\n");

	//agrv[1] 파일을 삭제한다.
	if(remove(argv[1]) < 0) {
		fprintf(stderr, "remove error\n");
		remove(argv[2]);
		exit(1);
	}
	else
		printf("step2 passed.\n");

	printf("Success!\n");
	exit(0);
}
