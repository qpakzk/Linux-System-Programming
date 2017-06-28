#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(void) {
	char buf[BUFFER_SIZE];
	int a, b;
	int i;

	//stdin을 BUFFER_SIZE 만큼 설정 
	setbuf(stdin, buf);
	scanf("%d %d", &a, &b);

	//new line을 만나기 전까지 출력
	for(i = 0; buf[i] != '\n'; i++)
		putchar(buf[i]);

	putchar('\n');
	exit(0);
}
