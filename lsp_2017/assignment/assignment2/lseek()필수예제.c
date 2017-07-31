#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(void) {
	char *fname = "ssu_test.txt";
	off_t fsize;
	int fd;

	if((fd = open(fname, O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

    //lseek()을 file size를 구한다.
	if((fsize = lseek(fd, 0, SEEK_END)) < 0) {
		fprintf(stderr, "lseek error\n");
		exit(1);
	}
    
    //printf("The size of <%s> is %lu bytes.\n", fname, fsize);
	printf("The size of <%s> is %lld bytes.\n", fname, fsize);

	exit(0);
}
