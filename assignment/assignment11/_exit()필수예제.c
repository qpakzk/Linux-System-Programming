#include <stdio.h>
#include <unistd.h>

int main(void) {
	printf("Good afternoon?");
	//표준 입출력 버퍼를 비우지 않고 종료
	_exit(0);
}
