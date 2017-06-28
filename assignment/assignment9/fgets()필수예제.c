#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void) {
	char buf[BUFFER_SIZE];

	//표준입력으로부터 입력을 받음
	while(fgets(buf, BUFFER_SIZE, stdin) != NULL)
		//표준출력으로 출력
		if(fputs(buf, stdout) == EOF) {
			fprintf(stderr, "standard output error\n");
			exit(1);
		}

	//표준입력 오류 체크
	if(ferror(stdin)) {
		fprintf(stderr, "standard input error\n");
		exit(1);
	}

	exit(0);
}
