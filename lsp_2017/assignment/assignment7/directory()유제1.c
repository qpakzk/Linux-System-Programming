#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>

#ifdef PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

#define MAX_PATH_GUESSED 1024

#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

char *pathname;
char command[LINE_MAX], grep_cmd[LINE_MAX];

//grep 명령 실행
int ssu_do_grep(void) {
	struct dirent *dirp;
	struct stat statbuf;
	char *ptr;
	DIR *dp;
	
	//파일 정보 추출
	if(lstat(pathname, &statbuf) < 0) {
		fprintf(stderr, "lstat error for %s\n", pathname);
		return 0;
	}	

	//디렉터리가 아니면
	if(S_ISDIR(statbuf.st_mode) == 0) {
		//grep 명령과 경로정보를 결합
		sprintf(command, "%s %s", grep_cmd, pathname);
		printf("%s : \n", pathname);
		//grep 명령 실행
		system(command);
		return 0;
	}

	ptr = pathname + strlen(pathname);
	*ptr++ = '/';
	*ptr = '\0';

	//pathname 디렉터리 open
	if((dp = opendir(pathname)) == NULL) {
		fprintf(stderr, "opendir error for %s\n", pathname);
		return 0;
	}

	//디렉터리 read
	while((dirp = readdir(dp)) != NULL)
		//현재 디렉터리, 부모 디렉터리가 아닐경우
		if(strcmp(dirp->d_name, ".") && strcmp(dirp->d_name, "..")) {
			strcpy(ptr, dirp->d_name);

			if(ssu_do_grep() < 0)
				break;
		}

	ptr[-1] = 0;
	closedir(dp);
	return 0;
}

//grep 명령 생성
void ssu_make_grep(int argc, char *argv[]) {
	int i;
	strcpy(grep_cmd, " grep");

	for(i = 1; i < argc; i++) {
		strcat(grep_cmd, " ");
		strcat(grep_cmd, argv[i]);
	}
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "usage: %s <-CVbchilnsvwx> <-num> <-A num> <-B num> <-f file> \n"
						"		   <-e> expr <directory>\n", argv[0]);
		exit(1);
	}

	if(pathmax == 0) {
		//최대경로개수 반환
		if((pathmax = pathconf("/", _PC_PATH_MAX)) < 0)
			pathmax = MAX_PATH_GUESSED;// /(root)가 현재 작업 디렉터리가 아니면
		else
			pathmax++;
	}

	if((pathname = (char *)malloc(pathmax + 1)) == NULL) {
		fprintf(stderr, "malloc error\n");
		exit(1);
	}

	//pathname에 경로 저장
	strcpy(pathname, argv[argc - 1]);
	//grep 명령 생성
	ssu_make_grep(argc, argv);
	//grep 명령 실행
	ssu_do_grep();
	exit(0);
}
