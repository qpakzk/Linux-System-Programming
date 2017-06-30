#include "queue.h"
//새로운 노드를 생성하는 함수
Node* newNode(pid_t pid) {
	Node *node = (Node *)malloc(sizeof(Node));
	node->pid = pid;
	node->next = NULL;
	return node; 
}

//queue를 초기화하는 함수
void init_queue(Queue *q) {
	q = (Queue *)malloc(sizeof(Queue));
	q->front = NULL;
	q->rear = NULL;
	q->count = 0;
}

//queue의 rear에서 데이터를 추가하는 함수
void enqueue(Queue *q, pid_t pid) {
	//최대 크기에 도달하면 해당 프로세스에게 SIGKILL 시그널 전송
	if(q->max_size <= q->count) {
		kill(pid, SIGKILL);
		return;
	}
	//새 노드 생성
	Node *temp = newNode(pid);
	q->count++;
	//처음으로 노드를 추가하는 경우
	if (q->rear == NULL) {
		q->front = temp;
		q->rear = temp;
		return;
	}

	//rear에 새 노드 추가
	q->rear->next = temp;
	q->rear = temp;
}

//queue의 front에서 데이터를 제거하는 함수
pid_t dequeue(Queue *q) {
	pid_t pid;
	if (q->front == NULL)
		return -1;
	//front에서 노드 삭제
	Node *temp = q->front;
	q->front = q->front->next;
	q->count--;

	//노드를 전부 삭제한 경우
	if (q->front == NULL)
		q->rear = NULL;

	pid = temp->pid;
	free(temp);
	temp = NULL;
	
	//삭제된 노드 pid 반환
	return pid;
}

//비어있는 queue인지 확인
bool empty(Queue *q) {
	return (q->count == 0);
}

