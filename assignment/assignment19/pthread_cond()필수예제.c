#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);

//뮤텍스 변수 초기화
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
//조건 변수 초기화
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

int count = 0;
int input = 0;
int t1 = 0, t2 = 0;

int main(void) {
	pthread_t tid1, tid2;
	int status;

	//스레드 생성
	if(pthread_create(&tid1, NULL, ssu_thread1, NULL) != 0) {
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}	

	if(pthread_create(&tid2, NULL, ssu_thread2, NULL) != 0) {
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}

	while(1) {
		printf("2개 이상의 개수 입력 : ");
		scanf("%d", &input);

		if(input >= 2) {
			//cond1을 기다리고 있는 스레드를 재시작
			pthread_cond_signal(&cond1);
			break;
		}
	}	

	//스레드가 종료할 때까지 대기
	pthread_join(tid1, (void *)&status);
	pthread_join(tid2, (void *)&status);
	printf("complete\n");
	exit(0);
}

void *ssu_thread1(void *arg) {
	while(1) {
		//lock
		pthread_mutex_lock(&mutex1);

		if(input < 2)//cond1을 받을 때까지 대기
			pthread_cond_wait(&cond1, &mutex1);

		if(input == count) {
            //cond2를 기다리고 있는 스레드를 재시작
			pthread_cond_signal(&cond2);
			break;
		}

		if(count == 0) {
			t2++;
			count++;
			printf("Thread 1 : %d\n", t1);
		}
		else if(count % 2 == 0) {
			t1 += t2;
			count++;
			printf("Thread 1 : %d\n", t1);
		}

		//cond2를 기다리고 있는 스레드를 재시작
		pthread_cond_signal(&cond2);
		//cond1을 받을 때까지 대기
		pthread_cond_wait(&cond1, &mutex1);
		//unlock
		pthread_mutex_unlock(&mutex1);
	}

	return NULL;
}

void *ssu_thread2(void *arg) {
	while(1) {
		//lock
		pthread_mutex_lock(&mutex2);

		if(input < 2)
			//cond2를 받을 때까지 대기
			pthread_cond_wait(&cond2, &mutex2);

		if(input == count) {
			//cond1을 기다리고 있는 스레드를 재시작
			pthread_cond_signal(&cond1);
			break;
		}

		if(count == 1) {
			count++;
			printf("Thread 2 : %d\n", t2);
		}
		else if(count % 2 == 1) { 
			t2 += t1;
			count++;
			printf("Thread 2 : %d\n", t2);
		}

		//cond1을 기다리고 있는 스레드를 재시작
		pthread_cond_signal(&cond1);
		//cond2를 받을 때까지 대기
		pthread_cond_wait(&cond2, &mutex2);
		//unlock
		pthread_mutex_unlock(&mutex2);
	}

	return NULL;
}
