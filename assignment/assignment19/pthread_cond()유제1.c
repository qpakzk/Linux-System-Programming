#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>

//뮤텍스 변수 초기화
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//조건 변수 초기화
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//공유 변수
int glo_val1 = 1, glo_val2 = 2;

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);

int main(void) {
	pthread_t tid1, tid2;

	//스레드 생성
	pthread_create(&tid1, NULL, ssu_thread1, NULL);
	pthread_create(&tid2, NULL, ssu_thread2, NULL);
	//스레드가 종료할 때까지 대기
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	//뮤텍스 변수 해제
	pthread_mutex_destroy(&lock);
	//조건 변수 해제
	pthread_cond_destroy(&cond);

	exit(0);
}

void *ssu_thread1(void *arg) {
	sleep(1);
	glo_val1 = 2;
	glo_val2 = 1;

	if(glo_val1 > glo_val2) //cond를 기다리고 있는 모든 스레드를 재시작
		pthread_cond_broadcast(&cond);

	printf("ssu_thread1 end\n");
	return NULL;
}

void *ssu_thread2(void *arg) {
	struct timespec timeout;//nano sec
	struct timeval now;//macro sec

	//lock
	pthread_mutex_lock(&lock);
	//시간을 now에 저장
	gettimeofday(&now, NULL);

	timeout.tv_sec = now.tv_sec + 5;
	timeout.tv_nsec = now.tv_usec * 1000;

	if(glo_val1 <= glo_val2) {
		printf("ssu_thread2 sleep\n");
		
		//정해진 기간까지 cond를 기다림
		if(pthread_cond_timedwait(&cond, &lock, &timeout) == ETIMEDOUT)
			printf("timeout\n");//기한이 지난 경우
		else//기한 내에 cond를 받은 경우
			printf("glo_val1 = %d, glo_val2 = %d\n", glo_val1, glo_val2);	
	}

	//unlock
	pthread_mutex_unlock(&lock);
	printf("ssu_thread2 end\n");
	return NULL;
}	
