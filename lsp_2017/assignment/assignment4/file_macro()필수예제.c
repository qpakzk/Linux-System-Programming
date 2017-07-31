#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
	struct stat file_info;
	char *str;
	int i;

	for(i = 1; i < argc; i++) {
		printf("name = %s, type = ", argv[i]);

		//file_info에 파일 정보 저장
		if(lstat(argv[i], &file_info) < 0) {
			fprintf(stderr, "lstat error\n");
			continue;
		}

		//파일 종류 구분
		if(S_ISREG(file_info.st_mode)) 
			str = "regular";
		else if(S_ISDIR(file_info.st_mode))
			str = "directory";
		else if(S_ISCHR(file_info.st_mode))
			str = "character special";
		else if(S_ISBLK(file_info.st_mode))
			str = "block special";
		else if(S_ISFIFO(file_info.st_mode))
			str = "FIFO";
		else if(S_ISLNK(file_info.st_mode))
			str = "symbolic link";
		else if(S_ISSOCK(file_info.st_mode))
			str = "socket";
		else
			str = "unknown mode";

		printf("%s\n", str);
	}

	exit(0);
}
