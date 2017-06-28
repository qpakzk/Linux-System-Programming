#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

struct ssu_msgbuf {
	char msg_text[BUFFER_SIZE];
	long msg_type;
};

int main(void) {
	struct ssu_msgbuf buf;
	key_t key;
	int msg_queueid;
	
	//키 생성
	if((key = ftok("ssu_dummy.c", 'B')) == -1) {
		fprintf(stderr, "ftok error\n");
		exit(1);
	}	

	//메시지 큐 생성
	if((msg_queueid = msgget(key, 0644 | IPC_CREAT)) == -1) {
		fprintf(stderr, "msgget error\n");
		exit(1);
	}

	printf("Enter lines of text, ^D to quit: \n");
	buf.msg_type = 1;

	//표준입력으로 입력을 받음
	while(fgets(buf.msg_text, sizeof(buf.msg_text), stdin) != NULL) {
		int length = strlen(buf.msg_text);

		//개행문자를 문자열에서 제거
		if(buf.msg_text[length - 1] == '\n')
			buf.msg_text[length - 1] = '\0';

		//메시지 큐에 메시지를 보냄
		if(msgsnd(msg_queueid, &buf, length + 1, 0) == -1)
			fprintf(stderr, "msgsnd error");
	}

	//메시지 큐 제거
	if(msgctl(msg_queueid, IPC_RMID, NULL) == -1) {
		fprintf(stderr, "msgctl error");
		exit(1);
	}

	exit(0);
}
