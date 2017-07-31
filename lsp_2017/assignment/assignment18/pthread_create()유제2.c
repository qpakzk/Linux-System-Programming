#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_thread(void *arg);

pthread_t glo_tid;

int main(void) {
	pthread_t loc_tid;
    
    //thread 생성
	if(pthread_create(&loc_tid, NULL, ssu_thread, NULL) != 0) {
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}

	sleep(5);
    
    //thread id가 동일한지 비교
	if(pthread_equal(loc_tid, glo_tid) == 0) {
		printf("다른 스레드\n");
		exit(0);
	}

	printf("동일한 스레드\n");
	exit(0);
}

void *ssu_thread(void *arg) {
	printf("스레드에서 자신의 스레드 ID를 전역변수에 할당\n");
    //thread id
	glo_tid = pthread_self();
	return NULL;
}
