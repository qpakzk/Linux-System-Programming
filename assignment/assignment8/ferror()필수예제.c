#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	FILE *fp;
	int character;

	if(argc != 2) {
		fprintf(stderr, "usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	//argv[1] 파일을 읽기 모드로 open
	if((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", argv[1]);
		exit(1);
	}

	//argv[1] 파일의 문자 하나를 character에 저장
	character = fgetc(fp);

	//파일의 끝일 때까지
	while(!feof(fp)) {
		//표준 출력으로 출력
		fputc(character, stdout);

		//fp의 읽기에 에러가 있는지 판단
		if(ferror(fp)) {
			fprintf(stderr, "Error detected!!\n");
			clearerr(fp);
		}

		//argv[1] 파일의 문자 하나를 character에 저장
		character = fgetc(fp);
	}

	//argv[1] 파일을 close
	fclose(fp);
	exit(0);
}
