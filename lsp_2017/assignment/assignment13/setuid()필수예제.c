#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	char line[256];
	FILE *fp;
	int uid;

	if(argc < 2) {
		fprintf(stderr, "usage: %s file_name\n", argv[0]);
		exit(1);
	}
	
	//실제 사용자 ID, 유효 사용자 ID
	printf("initially uid = %d and euid = %d\n", getuid(), geteuid());
	fp = fopen(argv[1], "r");

	if(fp == NULL) {
		fprintf(stderr, "first open error for %s\n", argv[1]);
		exit(1);
	}
	else {
		printf("first open successful:\n");

		//argv[1] 데이터를 line 배열에 저장하고 표준출력	
		while(fgets(line, 255, fp) != NULL)
			fputs(line, stdout);

		fclose(fp);
	}

	
	//자신의 실제 사용자 ID로 유효 사용자 ID를 변경하여
	//user1 사용자가 실행시키지 못함
	setuid(uid = getuid());
	printf("after setuid(%d):\n uid=%d and euid=%d\n", uid, getuid(), geteuid());
	fp = fopen(argv[1], "r");
	if(fp == NULL) {
		fprintf(stderr, "second open error for %s\n", argv[1]);
		exit(1);
	}
	else {
		printf("second open successful:\n");

		while(fgets(line, 255, fp) != NULL)
			fputs(line, stdout);

		fclose(fp);
	}
	exit(0);
}
