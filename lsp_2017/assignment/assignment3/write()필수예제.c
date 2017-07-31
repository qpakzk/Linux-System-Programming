#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void) {
	char buf[BUFFER_SIZE];
	int length;
    
    //표준 입력으로 BUFFER_SIZE만큼 buf에 read한 내용 저장
    //read한 바이트 수를 length에 반환
	length = read(0, buf, BUFFER_SIZE);
    //표준 출력으로 buf 내용을 출력
	write(1, buf, length);
	exit(0);
}
