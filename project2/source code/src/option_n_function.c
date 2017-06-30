#ifndef __OPTION_N_FUNCTION_C__
#define __OPTION_N_FUNCTION_C__

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "option_setting.h"
#include "common.h"
#include "option_n_function.h"

//-n 옵션을 실행시키는 함수
void execute_option_n(struct option_data *opt_info, int p_path, int opt_r) {	
	int i;
	pid_t my_pid;
	char str_pid[256];
	char path[1024]; 
	if(p_path) {//print path
		if(opt_info->num == 0) {//No pid
			my_pid = getpid();					
			trans_int_to_str(my_pid, str_pid);
			make_proc_io(path, str_pid);
			printf("([%s])\n", path);
			if(check_access(path) == -1)	
				return;
			print_io_info(path);
		}
		else if(opt_info->num == 1) {//one pid
			strcpy(str_pid, opt_info->list[0]);
			make_proc_io(path, str_pid);
			printf("([%s])\n", path);
			if(check_access(path) == -1)	
				return;
			print_io_info(path);
		}
		else if(opt_info->num >= 2) {
			max_argument(opt_info);
			if(opt_r)
				increasing_pid(opt_info);
			for(i = 0; i < opt_info->num; i++) {
				strcpy(str_pid, opt_info->list[i]);
				make_proc_io(path, str_pid);
				printf("([%s])\n", path);
				if(check_access(path) == -1)
					continue;
				print_io_info(path);
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

		make_proc_io(path, str_pid);
		if(check_access(path) == -1)	
			return;
		print_io_info(path);
	}
}

// /proc/[PID]/io를 생성하는 함수
void make_proc_io(char *file, char *pid) {
	strcpy(file, "/proc/");
	strcat(file, pid);
	strcat(file, "/io");
}

//io 정볼를 출력하는 함수
void print_io_info(const char *file) {
	FILE *fp = fopen(file, "r");
	char buf[1024];
	char io[7][128];
	int i, j, k;

	if(fp == NULL) {
		printf("fopen error for %s\n", file);
		return;
	}
		
	// /proc/[PID]/io 파일 정보를 buf에 저장
	fread(buf, sizeof(buf), sizeof(char), fp);

	j = 0;
	k = 0;

	//줄 단위로 파싱하여 io배열에 저장
	for(i = 0; i < (int) strlen(buf); i++) {
		if(buf[i] == '\n') {
			io[j][k] = 0;
			j++;
			if(j == 7)
				break;
			k = 0;
			continue;
		}
		io[j][k] = buf[i];
		k++;
	}
		
	io_arrangement(io);
	//io 파일 정보 출력
	for(i = 0; i < 7; i++)
		printf("%s\n", io[i]);

	fclose(fp);
}

//명세서에 요구하는 형식으로 변경하는 함수
void io_arrangement(char io[][128]) {
	char temp[7][128];
	int i, j, k, m;
	
	strcpy(temp[0], "Characters read : ");
	strcpy(temp[1], "Characters written : ");
	strcpy(temp[2], "Read syscalls : ");
	strcpy(temp[3], "Write syscalls : ");
	strcpy(temp[4], "Bytes read : ");
	strcpy(temp[5], "Bytes written : ");
	strcpy(temp[6], "Cancelled write bytes : ");
	for(i = 0; i < 7; i++) {
		j = 0;
		while(1) {
			if(isdigit(io[i][j])) 
				break;
			j++;
		}
		
		m = strlen(temp[i]);
		for(k = j; k < (int) strlen(io[i]); k++) {
			temp[i][m] = io[i][k];
			m++;
		}
		temp[i][m] = '\0';
		strcpy(io[i], temp[i]);
	}
}

#endif
