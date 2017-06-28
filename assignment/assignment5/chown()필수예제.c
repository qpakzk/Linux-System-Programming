#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void) {
	struct stat statbuf;
	char *fname = "ssu_myfile";
	int fd;

	if((fd = open(fname, O_RDWR | O_CREAT, 0600)) < 0) {
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}	
	
	close(fd);
	stat(fname, &statbuf);
    //root 권한으로 실행시키면 uid, gid가 각각 0, 0이 출력됨
	printf("# 1st stat buf call # UID:%d  GID:%d\n", statbuf.st_uid, statbuf.st_gid);

    //uid, gid 변경
	if(chown(fname, 501, 100) < 0) {
		fprintf(stderr, "chown error for %s\n", fname);
		exit(1);	
	}

	stat(fname, &statbuf);
    //변경된 결과 출력
	printf("# 2nd stat buf call # UID:%d  GID:%d\n", statbuf.st_uid, statbuf.st_gid);

    //unlink를 통해 link count 감소
	if(unlink(fname) < 0) {
		fprintf(stderr, "unlink error for %s\n", fname);
		exit(1);
	}

	exit(0);
}
