#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void) {
	char buf[BUFFER_SIZE];

	//stdout의 버퍼를 BUFFER_SIZE 만큼 설정
	setbuf(stdout, buf);
	//버퍼에 대기
	printf("Hello, ");
	sleep(1);
	//버퍼에 대기
	printf("OSLAB!!");
	sleep(1);
	//printf()는 line buffering 함수이므로
	//new line을 만나 버퍼에 있는 데이터를 한꺼번에 출력
	printf("\n");
	sleep(1);

	//Non Buffering
	setbuf(stdout, NULL);
	//즉시 출력
	printf("How");
	sleep(1);
	printf(" are");
	sleep(1);
	printf(" you?");
	sleep(1);
	printf("\n");
	exit(0);
}
