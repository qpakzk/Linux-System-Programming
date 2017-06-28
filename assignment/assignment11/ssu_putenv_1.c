#include <stdio.h>
#include <stdlib.h>

void ssu_printenv(char *label, char ***envpp);

//환경변수 전체에 접근하는 전역변수
extern char **environ;

//환경변수 전체에 접근하는 main()의 세번째 인자 
int main(int argc, char *argv[], char *envp[]) {
	ssu_printenv("Initially", &envp);
	//환경변수 값 변경 
	putenv("TZ=PST8PDT");
	//환경변수 값 변경 후 출력
	ssu_printenv("After changing TZ", &envp);
	//환경변수 등록
	putenv("WARNING=Don't use envp after putenv()");
	//환경변수 등록 후 출력
	ssu_printenv("After setting a new variable", &envp);
	//환경변수 WARNING의 값 출력
	printf("value of WARNING is %s\n", getenv("WARNING"));
	exit(0);
}

void ssu_printenv(char *label, char ***envpp) {
	char **ptr;
	printf("---- %s ----\n", label);
	//주소를 8진수로 출력
	printf("envp is at %8o and contains %8o\n", envpp, *envpp);
	printf("environ is at %8o and contains %8o\n", &environ, environ);
	printf("My environment variable are:\n");
	//환경변수 출력
	for(ptr = environ; *ptr; ptr++)
		printf("(%8o) = %8o -> %s\n", ptr, *ptr, *ptr);

	printf("(%8o) = %8o\n", ptr, *ptr);
}
