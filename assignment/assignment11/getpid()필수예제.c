#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
	//프로세스 ID
	printf("Process ID = %d\n", getpid());
	//부모 프로세스 ID
	printf("Parent process ID = %d\n", getppid());
	//실제 사용자 ID
	printf("Real user ID = %d\n", getuid());
	//유효 사용자 ID
	printf("Effective user ID = %d\n", geteuid());
	//실제 그룹 ID
	printf("Real group ID = %d\n", getgid());
	//유효 그룹 ID
	printf("Effective group ID = %d\n", getegid());
	exit(0);
}
