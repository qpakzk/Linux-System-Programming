#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	//execv() 인자로 넘겨줄 배열 초기화
	char *argv[] = {
		"ssu_execl_test_1", "param1", "param2", (char *)0
	};	

	printf("this is the original program\n");
	//새로운 프로세스 실행 
	execv("./ssu_execl_test_1", argv);
	printf("%s\n", "This line should never get printed\n");
	exit(0);
}
