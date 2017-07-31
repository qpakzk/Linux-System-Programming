#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(void) {
	char buf[BUFFER_SIZE];
	//임시파일명이 저장될 배열 
	char name[L_tmpnam];
	FILE *fp;

	printf("Temporary filename <<%s>>\n", tmpnam(name));

	//임시파일 생성 
	if((fp = tmpfile()) == NULL) {
		fprintf(stderr, "tmpfile create error!!\n");
		exit(1);
	}	

	//임시파일에 문자열 출력
	fputs("create tmpfile success!!\n", fp);
	//파일 위치 지시자를 파일 처음 위치로 이동
	rewind(fp);
	//buf에 임시파일 내용 저장
	fgets(buf, sizeof(buf), fp);
	puts(buf);
	exit(0);
}
