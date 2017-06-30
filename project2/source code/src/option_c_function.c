#ifndef __OPTION_C_FUNCTION_C__
#define __OPTION_C_FUNCTION_C__

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "option_setting.h"
#include "common.h"
#include "option_c_function.h"

//-c 옵션을 실행시키는 함수
void execute_option_c(struct option_data *opt_info, int p_path, int opt_r) {	
	int i;
	pid_t my_pid;
	char str_pid[256];
	char path[1024];

	if(p_path) {//print path
		if(opt_info->num == 0) {//No pid
			my_pid = getpid();					
			trans_int_to_str(my_pid, str_pid);
			make_proc_cmdline(path, str_pid);
			printf("([%s])\n", path);
			if(check_access(path) == -1)	
				return;
			print_cmdline_info(path);
		}
		else if(opt_info->num == 1) {//one pid
			strcpy(str_pid, opt_info->list[0]);
			make_proc_cmdline(path, str_pid);
			printf("([%s])\n", path);
			if(check_access(path) == -1)	
				return;
			print_cmdline_info(path);
		}
		else if(opt_info->num >= 2) {
			max_argument(opt_info);
			if(opt_r)
				increasing_pid(opt_info);
			for(i = 0; i < opt_info->num; i++) {
				strcpy(str_pid, opt_info->list[i]);
				make_proc_cmdline(path, str_pid);
				printf("([%s])\n", path);
				if(check_access(path) == -1)
					continue;
				print_cmdline_info(path);
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

		make_proc_cmdline(path, str_pid);
		if(check_access(path) == -1)	
			return;
		print_cmdline_info(path);
	}
}

// /proc/[PID]/cmdline을 생성하는 함수
void make_proc_cmdline(char *file, char *pid) {
	strcpy(file, "/proc/");
	strcat(file, pid);
	strcat(file, "/cmdline");
}

//cmdline 정보를 출력하는 함수
void print_cmdline_info(const char *file) {
	FILE *fp = fopen(file, "r");
	char buf[2048];
	char argv[256][256];
	int size, i, j, k, argc, c;

	if(fp == NULL) {
		printf("fopen error for %s\n", file);
		return;
	}
	size = 0;	

	//buf에 /proc/[PID]/cmdline 정보 저장
	while(fread(&buf[size], 1, 1, fp) == 1)
		size++;
	buf[size] = 0;

	memset(argv, 0, sizeof(argv));

	j = 0;
	k = 0;
	// '/0' 단위로 나눠진 정보를 파싱하여 저장
	for(i = 0; i < size; i++) {
		if(buf[i] == '\0') {
			argv[j][k] = '\0';
			j++;
			k = 0;
			continue;
		}
		argv[j][k] = buf[i];
		k++;
	}
	argc = j;
	c = 0;
	//파싱한 정보 출력
	for(i = 0; i < argc; i++) {
		if(argv[i][0] != 0) {
			printf("argv[%d] = %s\n", c, argv[i]);
			c++;
		}
	}
	
	fclose(fp);
}

#endif
