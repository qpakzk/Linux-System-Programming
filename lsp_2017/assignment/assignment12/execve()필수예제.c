#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	//execve() 인자로 넘겨줄 배열 초기화
	char *argv[] = {
		"ssu_execl_test_1", "param1", "param2", (char *)0
	};

	//execve() 환경변수로 넘겨줄 배열 초기화
	char *env[] = {
		"NAME=value",
		"nextname=nextvalue",
		"HOME=/home/oslab",
		(char *)0
	};

	printf("this is the original program\n");
	//새로운 프로세스 실행
	execve("./ssu_execl_test_1", argv, env);
	printf("%s\n", "This line should never get printed\n");
	exit(0);
}
