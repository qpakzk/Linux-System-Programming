#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

//파일 소유자, 그룹 사용자, 다른 사용자 모두 read, write 가능
#define RW_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) 

int main(void) {
	char *fname1 = "ssu_file1";
	char *fname2 = "ssu_file2";

	umask(0);//umask값 = 0000

	//umask 값이 0000이므로 0666 모드의 파일 생성
	if(creat(fname1, RW_MODE) < 0) {
		fprintf(stderr, "creat error for %s\n", fname1);
		exit(1);
	}

	//umask값 = 0066
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	//umask값이  0066이므로 0600 모드의 파일 생성
	if(creat(fname2, RW_MODE) < 0) {
		fprintf(stderr, "creat error for %s\n",fname2);
		exit(1);
	}

	exit(0);
}
