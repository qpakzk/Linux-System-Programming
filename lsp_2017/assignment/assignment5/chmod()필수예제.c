#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void) {
	struct stat statbuf;
	char *fname1 = "ssu_file1";
	char *fname2 = "ssu_file2";

	if(stat(fname1, &statbuf) < 0)
		fprintf(stderr, "stat error %s\n",fname1);
	
	//fname1 모드에서 그룹 실행모드를 제거하고 set user id 비트 설정
	if(chmod(fname1, (statbuf.st_mode & ~S_IXGRP) | S_ISUID) < 0)
		fprintf(stderr, "chmod error %s\n", fname1);

	//fname2를 0645 모드로 변경
	if(chmod(fname2, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IXOTH) < 0)
		fprintf(stderr, "chmod error %s\n", fname2);

	exit(0);
}
