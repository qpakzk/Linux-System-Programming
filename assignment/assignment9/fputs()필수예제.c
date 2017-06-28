#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	char buf[BUFFER_SIZE];
	FILE *fp;

	if(argc != 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		exit(1);
	}	

	//읽기 쓰기가 가능하도록 파일 open
	if((fp = fopen(argv[1], "w+")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", argv[1]);
		exit(1);
	}

	//표준출력으로 출력
	fputs("Input String >> ", stdout);
	//표준입력으로부터 buf에 입력을 받음
	gets(buf);
	//fp가 가리키는 파일에 출력
	fputs(buf, fp);
	//파일 위치 지시자를 파일 처음으로 설정
	rewind(fp);
	//fp가 가리키는 파일로 부터 buf에 입력을 받음
	fgets(buf, sizeof(buf), fp);
	//표준출력에 buf 출력
	puts(buf);
	fclose(fp);
	exit(0);
}
