#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 4096

int main(void) {
	char buf[MAX_LINE];
	//임시 파일의 이름이 저장될 문자배열 선언
	char name[L_tmpnam];
	FILE *fp;

	//tmpnam()이 임시파일명 반환하여 출력
	printf("temp file 1 : %s\n", tmpnam(NULL));
	//임시파일명을 name네 저장
	tmpnam(name);
	printf("temp file 2 : %s\n", name);

	//임시파일 생성
	if((fp = tmpfile()) == NULL) {
		fprintf(stderr, "tmpfile error\n");
		exit(1);
	}

	//임시 파일에 문자열 출력
	fputs("tmpfile created temporary file.\n", fp);
	//fp를 통해 임시파일 접근
	fseek(fp, 0, SEEK_SET);
	
	//임시 파일에 있는 내용을 buf에 저장
	if(fgets(buf, sizeof(buf), fp) == NULL) {
		fprintf(stderr, "fgets error\n");
		exit(1);
	}

	//임시파일 내용을 표준출력으로 출력
	fputs(buf, stdout);
	exit(0);
}
