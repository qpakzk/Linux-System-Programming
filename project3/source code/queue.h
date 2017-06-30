#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>

//Node 구조체
typedef struct _node {
	pid_t pid;//프로세스 id
	struct _node *next;//LinkedList로 구현하는 queue
} Node;

//LinkedList로 구현하는 queue
typedef struct _queue {
	//front : queue의 맨 앞 노드를 가리키는 포인터
	//rear : queue의 맨 뒤 노드를 가리키는 포인터
	Node *front, *rear;
	//노드의 개수
	int count;
	//최대로 저장할 수 있는 노드의 개수
	int max_size;
} Queue;

//새로운 노드를 생성하는 함수
Node* newNode(pid_t pid);
//queue를 초기화하는 함수
void init_queue(Queue *q);
//queue의 rear에서 데이터를 추가하는 함수
void enqueue(Queue *q, pid_t pid);
//queue의 front에서 데이터를 제거하는 함수
pid_t dequeue(Queue *q);
//비어있는 queue인지 확인
bool empty(Queue *q);

#endif
