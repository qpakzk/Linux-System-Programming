#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int character;

	//표준입력으로 입력받음
	while((character = getc(stdin)) != EOF) 
		//표준출력으로 출력
		if(putc(character, stdout) == EOF) {
			fprintf(stderr, "standard output error\n");
			exit(1);
		}
	
	//표준입력 에러 발생여부 판단
	if(ferror(stdin)) {
		fprintf(stderr, "standard input error\n");
		exit(1);
	}

	exit(0);
}
