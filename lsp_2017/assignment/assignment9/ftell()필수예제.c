#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *fname = "ssu_test.txt";
	FILE *fp;	
	long fsize;

	//읽기 전용으로 파일 open
	if((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	//파일 위치 지시자를 파일 맨 끝으로 이동
	fseek(fp, 0, SEEK_END);
	//파일 위치 지시자의 현재 위치를 반환하여 파일의 크기를 측정
	fsize = ftell(fp);
	printf("The size of <%s> is %ld bytes\n", fname, fsize);
	exit(0);
}
