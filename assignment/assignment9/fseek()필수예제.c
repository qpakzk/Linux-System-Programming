#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *fname = "ssu_test.txt";
	FILE *fp;
	long position;
	int character;

	//읽기 전용으로 파일 open
	if((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	printf("Input Number >>");
	scanf("%ld", &position);
	//파일의 처음 위치로부터 position - 1만큼 파일 위치 지시자를 이동
	fseek(fp, position - 1, SEEK_SET);
	//현재 파일 위치 지시자가 가리키고 있는 문자를 입력
	character = getc(fp);
	printf("%ldth character => %c\n", position, character);
	exit(0);
}
