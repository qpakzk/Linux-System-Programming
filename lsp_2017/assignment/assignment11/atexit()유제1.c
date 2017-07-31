#include <stdio.h>
#include <stdlib.h>

void ssu_out(void);

int main(void) {
	//exit() 실행 시 호출될 함수 등록
	if(atexit(ssu_out)) {
		fprintf(stderr, "atexit error\n");
		exit(1);
	}

	exit(0);
}

void ssu_out(void) {
	printf("atexit succeeded!\n");
}
