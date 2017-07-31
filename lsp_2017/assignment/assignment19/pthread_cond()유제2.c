#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define VALUE_DONE 10
#define VALUE_STOP1 3
#define VALUE_STOP2 6

//뮤텍스 변수 초기화
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//조건 변수 초기화
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);

//공용 변수
int glo_val = 0;

int main(void) {
	pthread_t tid1, tid2;

	//스레드 생성
	pthread_create(&tid1, NULL, &ssu_thread1, NULL);	
	pthread_create(&tid2, NULL, &ssu_thread2, NULL);	
	//스레드가 종료될 때까지 대기
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	printf("final value: %d\n", glo_val);
	exit(0);
}

void *ssu_thread1(void *arg) {
	while(1) {
		//lock
		pthread_mutex_lock(&lock);
		//cond를 받을 때까지 대기
		pthread_cond_wait(&cond, &lock);
		glo_val++;
		printf("global value ssu_thread1: %d\n", glo_val);
		//unlock
		pthread_mutex_unlock(&lock);

		if(glo_val >= VALUE_DONE)
			return NULL;
	}
}

void *ssu_thread2(void *arg) {
	while(1) {
		//lock
		pthread_mutex_lock(&lock);
		if(glo_val < VALUE_STOP1 || glo_val > VALUE_STOP2) 
			pthread_cond_signal(&cond);//cond를 기다리고 있는 스레드 중 하나를 재시작
		else {
			glo_val++;
			printf("global value ssu_thread2: %d\n", glo_val);
		}

		//unlock
		pthread_mutex_unlock(&lock);

		if(glo_val >= VALUE_DONE)
			return NULL;
	}
}
