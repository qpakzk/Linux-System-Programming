#include <stdio.h>
#include <stdlib.h>

//argc : shell 상에서 인자로 입력받은 문자열의 개수
//argv : shell 상에서 인자로 입력받은 문자열을 가리키는 포인터
int main(int argc, char *argv[]) {
	int i;

	//입력받은 문자열을 출력
	for(i = 0; i < argc; i++) 
		printf("argv[%d] : %s\n", i, argv[i]);

	exit(0);
}
