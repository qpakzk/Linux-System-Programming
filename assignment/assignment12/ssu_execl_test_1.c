#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	//환경변수에 접근하는 전역변수
	extern char **environ;
	char **str;
	int i;

	for(i = 0; i < argc; i++)
		printf("argv[%d]: %s\n", i, argv[i]);

	//환경변수 출력
	for(str = environ; *str != 0; str++)
		printf("%s\n", *str);

	exit(0);
}
