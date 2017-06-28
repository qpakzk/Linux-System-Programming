#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *fname = "ssu_test.txt";

	printf("First printf : Hello, OSLAB!!\n");

	//freopen() 호출로 표준출력이 ssu_test.txt의 쓰기로 바뀌어
	//printf()의 문자열이 ssu_test.txt에 출력된다.
	if(freopen(fname, "w", stdout) != NULL)
		printf("Second printf : Hello, OSLAB!!\n");

	exit(0);
}
