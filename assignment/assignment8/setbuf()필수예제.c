#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void) {
	char buf[BUFFER_SIZE];

	//stdout 버퍼를 BUFFER_SIZE 만큼 설정
	setbuf(stdout, buf);
	//버퍼에 저장 
	printf("Soongsil ");
	sleep(1);
	//버퍼에 저장
	printf("University!");
	sleep(1);
	//printf()가 line buffering 함수이므로
	//new line을 만났을 때 버퍼에 담겨있는 데이터를 한 번에 출력
	printf("\n");
	sleep(1);

	//Non buffered로 설정
	setbuf(stdout, NULL);
	//즉시 출력
	printf("Computer ");
	sleep(1);
	printf("Science & ");
	sleep(1);
	printf("Engineering");
	sleep(1);
	printf("\n");
	exit(0);
}
