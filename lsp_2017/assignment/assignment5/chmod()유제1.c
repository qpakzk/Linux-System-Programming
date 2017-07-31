#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

//파일 소유자, 그룹 사용자, 다른 사용자 실행모드
#define MODE_EXEC (S_IXUSR | S_IXGRP | S_IXOTH)

int main(int argc, char *argv[]) {
	struct stat statbuf;
	int i;

	if(argc < 2) {
		fprintf(stderr, "usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
		exit(1);
	}

	for(i = 0; i < argc; i++) {
		if(stat(argv[i], &statbuf) < 0) {
			fprintf(stderr, "%s : stat error\n", argv[i]);
			continue;
		}

        //MODE_EXEC 모드를 파일 모드에 추가
		statbuf.st_mode |= MODE_EXEC;
        //그룹 사용자 실행권, 다른 사용자 실행권한 삭제
		statbuf.st_mode ^= (S_IXGRP | S_IXOTH);
        //수정된 파일모드로 변경시도
		if(chmod(argv[i], statbuf.st_mode) < 0) 
			fprintf(stderr, "%s : chmod error\n", argv[i]);
		else
			printf("%s : file permission was changed.\n", argv[i]);
	}

	exit(0);
}
