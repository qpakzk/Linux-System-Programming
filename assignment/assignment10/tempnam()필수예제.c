#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	char *arg_directory = NULL;
	char *arg_prefix = NULL;

	if(argc != 3) {
		fprintf(stderr, "usage: %s <directory> <prefix>\n", argv[0]);
		exit(1);
	}

	//사용자로부터 임시파일을 생성할 디렉터리를 입력받음
	//공백을 입력받는 경우 NULL을 저장하여 시스템이 설정
	arg_directory = argv[1][0] != ' ' ? argv[1] : NULL;
	//사용자로부터 임시파일의 접두어를 입력받음
	arg_prefix = argv[2][0] != ' ' ? argv[2] : NULL;
	//사용자 설정 임시파일 이름 반환
	printf("created : %s\n", tempnam(arg_directory, arg_prefix));
	exit(0);
}
