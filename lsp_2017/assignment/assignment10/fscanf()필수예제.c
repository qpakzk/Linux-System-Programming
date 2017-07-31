#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(void) {
	char *fname = "ssu_test.dat";
	char name[BUFFER_SIZE];
	FILE *fp;
	int age;

	//읽기 전용으로 파일 open
	fp = fopen(fname, "r");
	//fp가 가리키는 파일로부터 입력
	fscanf(fp, "%s%d", name, &age);
	fclose(fp);
	//쓰기 전용으로 파일 open
	fp = fopen(fname, "w");
	//fp가 가리키는 파일에 형식에 맞게 출력
	fprintf(fp, "%s is %d years old\n", name, age);
	fclose(fp);
	exit(0);
}
