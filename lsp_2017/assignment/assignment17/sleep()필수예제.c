#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void ssu_timestamp(char *str);

int main(void) {
	unsigned int ret;
	
	ssu_timestamp("before sleep()");	
	ret = sleep(10);//10초 간 프로세스 일시중지
	ssu_timestamp("after sleep()");
	printf("sleep() returned %d\n", ret);
	exit(0);
}

void ssu_timestamp(char *str) {
	time_t time_val;
	time(&time_val);
	//ssu_timestamp()가 호출되는 시점의 시각을 출력
	printf("%s the time is %s\n", str, ctime(&time_val));
}
