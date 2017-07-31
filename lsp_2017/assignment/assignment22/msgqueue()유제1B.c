#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
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
	if((key = ftok("ssu_msgqueue_1A.c", 'B')) == -1) {
		fprintf(stderr, "msgget error\n");
		exit(1);
	}	

    //메시지 큐 생성
	if((msg_queueid = msgget(key, 0644)) == -1) {
		fprintf(stderr, "msgget error\n");
		exit(1);
	}

	printf("spock: ready to receive messages, captain.\n");

	while(1) {
        //메시지 큐에서 메시지를 읽어들임
		if(msgrcv(msg_queueid, &buf, sizeof(buf.msg_text), 0, 0) == -1) {
			fprintf(stderr, "msgrcv: Identifier removed\n");
			exit(1);
		}

		printf("spock: \"%s\"\n", buf.msg_text);
	}
	exit(0);
}
