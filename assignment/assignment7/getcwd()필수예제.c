#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH_MAX 1024

int main(void) {
	char *pathname;

	// /home 디렉터리로 이동
	if(chdir("/home") < 0) {
		fprintf(stderr, "chdir error\n");
		exit(1);
	}

	pathname = malloc(PATH_MAX);

	//pathname에 현재 작업 디렉터리 저장
	if(getcwd(pathname, PATH_MAX) == NULL) {
		fprintf(stderr, "getcwd error\n");
		exit(1);
	}

	printf("current directory = %s\n", pathname);
	exit(0);
}
