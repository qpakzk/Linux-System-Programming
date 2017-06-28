#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHARED_MEMORY_SIZE 1024

int main(int argc, char *argv[]) {
	key_t key;
	char *data;
	int shared_memory_id;

	if(argc > 2) {
		fprintf(stderr, "usage: %s [data_to_write] \n", argv[0]);
		exit(1);
	}

	//키 획득
	if((key = ftok("ssu_shmdemo.c", 'R')) == -1) {
		fprintf(stderr, "ftok error\n");
		exit(1);
	}
	
	//공유메모리id  획득
	if((shared_memory_id = shmget(key, SHARED_MEMORY_SIZE, 0644 | IPC_CREAT)) == -1) {
		fprintf(stderr, "shmget error\n");
		exit(1);
	}

	//공유메모리를 프로세스 주소 공간의 일부로 설정
	if((data = shmat(shared_memory_id, (void *)0, 0)) == (char *)(-1)) {
		fprintf(stderr, "shmat error\n");
		exit(1);
	}

	if(argc == 2) {
		printf("writing to segment: \"%s\" \n", argv[1]);
		strncpy(data, argv[1], SHARED_MEMORY_SIZE);
	}
	else
		printf("segment contains: \"%s\" \n", data);

	//공유메모리를 프로세스 주소공간에서 분리
	if(shmdt(data) == -1) {
		fprintf(stderr, "shmdt error\n");
		exit(1);
	}
	
	exit(0);	
}
