#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	printf("this is the original program\n");
	//execl()에서 arg0 ~ arg3의 인자는 새로운 프로세스에서 main()의 argv인자로 들어간다.
	execl("./ssu_execl_test_1", "ssu_execl_test_1", "param1", "param2", "param3", (char *)0);
	printf("%s\n", "this line should never get printed\n");
	exit(0);
}
