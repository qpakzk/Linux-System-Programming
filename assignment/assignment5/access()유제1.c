#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TABLE_SIZE (sizeof(table)/sizeof(*table))

int main(int argc, char *argv[]) {
	struct {
		char *text;
		int mode;
	} table[] = {
		{"exists", 0},
		{"execute", 1},
		{"write", 2},
		{"read", 4}
	};
	int i;

	if(argc < 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		exit(1);
	}

    //argv[1] 파일이 존재하는지, 실행권한이 있는지, 쓰기권한이 있는지, 읽기권한이 있는지 판정
	for(i = 0; i < TABLE_SIZE; i++) {
		if(access(argv[1], table[i].mode) != -1)
			printf("%s -ok\n", table[i].text);
		else
			printf("%s\n", table[i].text);
	}

	exit(0);
}

