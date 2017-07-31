#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *fname = "ssu_test.txt";
	FILE *fp;

	//fname 파일을 읽기 권한으로 open
	if((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}
	else {
		printf("Success!\n");
		printf("Opening \"%s\" in \"r\" mode!\n", fname);
	}

	//fname 파일을 close한다.
	fclose(fp);
	exit(0);
}
