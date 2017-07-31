#ifndef __OPTION_T_FUNCTION_C__
#define __OPTION_T_FUNCTION_C__

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "option_setting.h"
#include "common.h"
#include "option_t_function.h"

//-t 옵션을 실행시키는 함수
void execute_option_t(struct option_data *opt_info, int p_path, int opt_r) {	
	int i;
	pid_t my_pid;
	char str_pid[256];
	char path[1024];

	if(p_path) {//print path

		if(opt_info->num == 0) {//No pid
			my_pid = getpid();					
			trans_int_to_str(my_pid, str_pid);
			make_proc_status(path, str_pid);
			printf("([%s])\n", path);
			if(check_access(path) == -1)	
				return;
			print_status_info(path);
		}
		else if(opt_info->num == 1) {//one pid
			strcpy(str_pid, opt_info->list[0]);
			make_proc_status(path, str_pid);
			printf("([%s])\n", path);
			if(check_access(path) == -1)	
				return;
			print_status_info(path);
		}
		else if(opt_info->num >= 2) {
			max_argument(opt_info);
			if(opt_r)
				increasing_pid(opt_info);
			for(i = 0; i < opt_info->num; i++) {
				strcpy(str_pid, opt_info->list[i]);
				make_proc_status(path, str_pid);
				printf("([%s])\n", path);
				if(check_access(path) == -1)
					continue;
				print_status_info(path);
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

		make_proc_status(path, str_pid);
		if(check_access(path) == -1)	
			return;
		print_status_info(path);
	}
}

// /proc/[PID]/status 파일을 생성하는 함수
void make_proc_status(char *file, char *pid) {
	strcpy(file, "/proc/");
	strcat(file, pid);
	strcat(file, "/status");
}

//status 정보를 출력하는 함수
void print_status_info(const char *file) {
	FILE *fp = fopen(file, "r");
	char buf[1024];
	char status[9][128];
	int i, j, k;

	if(fp == NULL) {
		printf("fopen error for %s\n", file);
		return;
	}
		
	// /proc/[PID]/status 파일의 모든 정보를 buf에 저장
	fread(buf, sizeof(buf), sizeof(char), fp);

	j = 0;
	k = 0;

	//줄 단위로 status 배열에 저장
	for(i = 0; i < (int) strlen(buf); i++) {
		if(buf[i] == '\n') {
			status[j][k] = 0;
			j++;
			if(j == 9)
				break;
			k = 0;
			continue;
		}
		status[j][k] = buf[i];
		k++;
	}
		
	//명세서의 요구조건에 맞게 변형
	status_arrangement(status);
	for(i = 0; i < 9; i++)
		printf("%s\n", status[i]);

	fclose(fp);
}

//명세서의 요구조건에 맞게 변형하는 함수
void status_arrangement(char status[][128]) {
	char temp[128];
	int i, j, it;
	char state;
	
	// '/t'를 ' '로 교체
	for(i = 0; i < 9; i++)
		for(j = 0; j < (int) strlen(status[i]); j++) {
			if(status[i][j] == '\t')
				status[i][j] = ' ';
		}

	//State
	i = 0;
	j = 0;
	
	//State 문자까지 이동
	while(1) {
		if(status[1][i] == ':') {
			temp[j] = status[1][i];
			i++;
			j++;
			temp[j] = ' ';
			j++;
			break;
		}
		else {
			temp[j] = status[1][i];
			i++;
			j++;
		}
	}
	temp[j] = '\0';	
	
	while(status[1][i] == ' ')
		i++;
	
	//State 문자 저장
	state = status[1][i];

	//State 문자을 해당 설명으로 교체하여 저장
	//테스트 환경(Ubuntu16.04)에 속하지 않는 State에 대해서는
	//케이스 분류하지 않음
	if(state == 'R')
		strcat(temp, "Running");
	else if(state == 'S') 
		strcat(temp, "Sleeping in an interruptible wait");
	else if(state == 'D')
		strcat(temp, "Waiting in uninterruptible disk sleep");
	else if(state == 'Z')
		strcat(temp, "Zombie");
	else if(state == 'T')
		strcat(temp, "Stopped");
	else if(state == 't')
		strcat(temp, "Tracing stop");
	else if(state == 'X')
		strcat(temp, "Dead");
	
	//변경된 정보로 교체 
	strcpy(status[1], temp);
	temp[0] = '\0';

	//Uid, Gid
	for(it = 7; it < 9; it++) {
		i = 0;
		j = 0;
		//Uid, Gid 번호까지 이동
		while(1) {
			if(status[it][i] == ':') {
				temp[j] = status[it][i];
				i++;
				j++;
				temp[j] = ' ';
				j++;
				break;
			}
			else {
				temp[j] = status[it][i];
				i++;
				j++;
			}
		}

		while(status[it][i] == ' ')
			i++;

		//번호 저장
		while(status[it][i] != ' ') {
			temp[j] = status[it][i];
			j++;
			i++;
		}

		temp[j] = '\0';
		//변경된 정보로 교체
		strcpy(status[it], temp);
		temp[0] = '\0';
	}
}

#endif
