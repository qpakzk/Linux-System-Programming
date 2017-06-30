#ifndef __OPTION_F_FUNCTION_C__
#define __OPTION_F_FUNCTION_C__

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "option_setting.h"
#include "common.h"
#include "option_f_function.h"

//-f 옵션을 실행시키는 함수
void execute_option_f(struct option_data *opt_info, int p_path, int opt_r) {	
	int i;
	pid_t my_pid;
	char str_pid[256];
	char path[1024];

	if(p_path) {//print path
		if(opt_info->num == 0) {//입력받은 pid가 없는 경우
			my_pid = getpid();//실행하는 프로세스 정보 이용
			trans_int_to_str(my_pid, str_pid);//정수를 문자열로 변환
			make_proc_fd_dir(path, str_pid);// /proc/str_pid/fd 생성
			printf("([%s])\n", path);//path 출력

			//접근권한 확인
			if(check_access(path) == -1)	
				return;
			
			//fd 정보를 출력
			search_n_print_fd_info(path, str_pid);
		}
		else if(opt_info->num == 1) {//입력받은 pid가 한 개인 경우
			strcpy(str_pid, opt_info->list[0]);
			make_proc_fd_dir(path, str_pid);
			printf("([%s])\n", path);
			if(check_access(path) == -1)	
				return;
			search_n_print_fd_info(path, str_pid);
		}
		else if(opt_info->num >= 2) {//입력받은 pid가 두 개 이상인 경우
			max_argument(opt_info);//최대 pid개수 초과여부 체크
			if(opt_r)//-r 옵션 설정 시
				increasing_pid(opt_info);//pid를 오름차순으로 정렬

			for(i = 0; i < opt_info->num; i++) {
				strcpy(str_pid, opt_info->list[i]);
				make_proc_fd_dir(path, str_pid);
				printf("([%s])\n", path);
				if(check_access(path) == -1)
					continue;
				search_n_print_fd_info(path, str_pid);
			}
		}
	}
	else {//No print path
		if(opt_info->num == 0) {//No pid
			my_pid = getpid();					
			trans_int_to_str(my_pid, str_pid);
		}
		else//one pid
			strcpy(str_pid, opt_info->list[0]);

		make_proc_fd_dir(path, str_pid);
		if(check_access(path) == -1)	
			return;
		search_n_print_fd_info(path, str_pid);
	}

}

// /proc/[PID]/fd 디렉터리를 생성하는 함수
void make_proc_fd_dir(char *dir, char *pid) {
	strcpy(dir, "/proc/");
	strcat(dir, pid);
	strcat(dir, "/fd");
}


// /proc/[PID]/fd/[fd number] 파일을 생성하는 함수
void make_proc_fd_file(char *dir, char *pid, char *fname) {
	strcpy(dir, "/proc/");
	strcat(dir, pid);
	strcat(dir, "/fd/");
	strcat(dir, fname);
}

//fd 정보를 출력하는 함수
void search_n_print_fd_info(char *dir, char *pid) {
	DIR *dirp;
	struct dirent *dent;
	int len;
	char path[1024];
	char actualpath[1024];

	if((dirp = opendir(dir)) != NULL) {
		while((dent = readdir(dirp)) != 0) {//모든 fd 번호를 탐색
			if(!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, "..")) 
				continue;
			printf("File Descripter number: %s, ", dent->d_name);
			make_proc_fd_file(path, pid, dent->d_name);
			//심볼릭링크의 실제경로 찾기
			len = readlink(path, actualpath, sizeof(actualpath));	
			actualpath[len] = 0;
			printf("Opened File: %s\n", actualpath);
		}
	}
	closedir(dirp);
}

#endif
