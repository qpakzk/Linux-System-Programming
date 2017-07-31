#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void ssu_addone(void);

//환경변수에 접근하는 전역변수
extern char **environ;

char glob_var[] = "HOBBY=swimming";

int main(void) {
	int i;
	
	//기존 환경변수 출력
	for(i = 0; environ[i] != NULL; i++) 
		printf("environ[%d] : %s\n", i, environ[i]);
	
	//전역변수에 저장된 환경변수 등록
	putenv(glob_var);
	ssu_addone();
	//환경변수 값에 대해 출력
	printf("My hobby is %s\n", getenv("HOBBY"));
	printf("My lover is %s\n", getenv("LOVER"));
	strcpy(glob_var + 6, "fishing");

	//환경변수 등록 후 환경변수 출력
	for(i = 0; environ[i] != NULL; i++)
		printf("environ[%d] : %s\n", i, environ[i]);

	exit(0);
}

void ssu_addone(void) {
	char auto_var[10];

	strcpy(auto_var, "LOVER=js");
	//지역변수에 저장된 환경변수 등록
	putenv(auto_var);
}
