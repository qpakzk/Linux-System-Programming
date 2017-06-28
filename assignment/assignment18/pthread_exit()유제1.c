#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_thread(void *arg);

int main(void) {
	pthread_t tid;

	//스레드 생성 
	if(pthread_create(&tid, NULL, ssu_thread, NULL) != 0) {
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}

	sleep(1);
	printf("스레드가 완료되기전 main 함수가 먼저 종료되면 실행중 스레드 소멸\n");
	printf("메인 종료\n");
	exit(0);
}

void *ssu_thread(void *arg) {
	printf("스레드 시작\n");
	sleep(5);
	printf("스레드 수행 완료\n");
	//스레드 종료
	pthread_exit(NULL);
	return NULL;
}
