#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

struct stat statbuf;

void ssu_checkfile(char *fname, time_t *time);

int main(int argc, char *argv[]) {
	time_t intertime;

    //shell을 통해 파일을 입력받음
	if(argc != 2) {
		fprintf(stderr, "usags: %s <file>\n", argv[0]);
		exit(1);
	}

    //stat()를 이용하여 statbuf에 파일 정보 저장
	if(stat(argv[1], &statbuf) < 0) {
		fprintf(stderr, "stat error for %s\n", argv[1]);
		exit(1);
	}

    //마지막으로 수정된 시간을 저장
	intertime = statbuf.st_mtime;
	while(1) {
		ssu_checkfile(argv[1], &intertime);
		sleep(10);
	}
}

void ssu_checkfile(char *fname, time_t *time) {
	if(stat(fname, &statbuf) < 0) {//argv[1]가 존재하지 않을 때
		fprintf(stderr, "Warning : ssu_checkfile() error!\n");
		exit(1);
	}
	else//argv[1]을 수정했을 때
		if(statbuf.st_mtime != *time) {//시간이 다르면 argv[1]이 수정되었다는 의미
			printf("Warning : %s was modified!.\n", fname);
			*time = statbuf.st_mtime;
		}
}
