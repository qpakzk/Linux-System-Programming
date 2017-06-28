#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *fname = "ssu_test.txt";
	char *mode = "r";

	//fname 파일을 읽기권한으로 open
	if(fopen(fname, mode) == NULL) {
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}
	else
		printf("Success!!\nFilename : <%s>, mode : <%s>\n", fname, mode);

	exit(0);
}
