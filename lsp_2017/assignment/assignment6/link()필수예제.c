#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "usage: %s <file1> <file2>\n", argv[0]);
		exit(1);
	}

	//argv[1] 파일을 argv[2] 이름으로 링크한다.
	if(link(argv[1], argv[2]) == -1) {
		fprintf(stderr, "link error for %s\n", argv[1]);
		exit(1);
	}

	exit(0);
}
