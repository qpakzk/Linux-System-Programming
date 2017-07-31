#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	printf("before system()\n");
	system("pwd");//print working directory 명령어 실행
	printf("after system()\n");
	exit(0);
}
