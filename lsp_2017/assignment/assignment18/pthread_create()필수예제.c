#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *ssu_thread(void *arg);

int main(void) {
	pthread_t tid;
	pid_t pid;

	//스레드 생성
	if(pthread_create(&tid, NULL, ssu_thread, NULL) != 0) {
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}

	//process id
	pid = getpid();
	//thread id
	tid = pthread_self();
	printf("Main Thread : pid %u tid %u\n",
			(unsigned int)pid, (unsigned int)tid);
	
	sleep(1);	
	exit(0);
}

//thread 함수
void *ssu_thread(void *arg) {
	pthread_t tid;
	pid_t pid;

	//process id
	pid = getpid();
	//thread id
	tid = pthread_self();
	printf("New Thread: pid %d tid %u\n", (int)pid, (unsigned int)tid);
	return NULL;
}
