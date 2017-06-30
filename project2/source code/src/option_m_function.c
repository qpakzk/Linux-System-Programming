#ifndef __OPTION_M_FUNCTION_C__
#define __OPTION_M_FUNCTION_C__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "option_setting.h"
#include "common.h"
#include "option_m_function.h"

//-m 옵션을 실행시키는 함수
void execute_option_m(struct option_data *opt_info, int p_path, int opt_r, struct option_data *opt_k_info) {	
	int i;
	pid_t my_pid;
	char str_pid[256];
	char path[1024];
	//-k 옵션 설정 여부 및 오류 체크
	int check = check_opt_k_info(opt_k_info, opt_info);

	if(p_path) {//print path
		if(opt_info->num == 0) {//No pid
			my_pid = getpid();					
			trans_int_to_str(my_pid, str_pid);
			make_proc_environ(path, str_pid);
			printf("([%s])\n", path);
			if(check_access(path) == -1)	
				return;
			print_environ_info(path, opt_k_info, check, opt_r);
		}
		else if(opt_info->num == 1) {//one pid
			strcpy(str_pid, opt_info->list[0]);
			make_proc_environ(path, str_pid);
			printf("([%s])\n", path);
			if(check_access(path) == -1)	
				return;
			print_environ_info(path, opt_k_info, check, opt_r);
		}
		else if(opt_info->num >= 2) {
			max_argument(opt_info);
			if(opt_r)
				increasing_pid(opt_info);
			for(i = 0; i < opt_info->num; i++) {
				strcpy(str_pid, opt_info->list[i]);
				make_proc_environ(path, str_pid);
				printf("([%s])\n", path);
				if(check_access(path) == -1)
					continue;
				print_environ_info(path, opt_k_info, check, opt_r);
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

		make_proc_environ(path, str_pid);
		if(check_access(path) == -1)	
			return;
		print_environ_info(path, opt_k_info, check, opt_r);
	}
}

// /proc/[PID]/environ을 생성하는 함수
void make_proc_environ(char *file, char *pid) {
	strcpy(file, "/proc/");
	strcat(file, pid);
	strcat(file, "/environ");
}

//environ 정보를 출력하는 함수
void print_environ_info(const char *file, struct option_data *opt_k_info, int check, int opt_r) {
	FILE *fp = fopen(file, "r");
	char environ[256][2048];
	int i, j;
	char e_var[256][2048] = {0};
	int size;
	int e_num;

	if(fp == NULL) {
		printf("fopen error for %s\n", file);
		return;
	}
	
	i = 0;
	j = 0;

	//environ 배열에 '\0'을 기준으로 파싱하면서 저장
	while(fread(&environ[i][j], 1, 1, fp) == 1) {
		if(environ[i][j] == '\0') {
			environ[i][j] = '\0';
			j = 0;
			i++;
			continue;
		}
		j++;
	}

	e_num = i;
	if(check == 0 || (check &&  opt_k_info->num == 0)) {//-k
		for(i = 0; i < e_num; i++)
			printf("%s\n", environ[i]);
	}
	else if(check && opt_k_info->num >= 1) {
		//-k 옵션 실행
		size = execute_option_k(opt_k_info, environ, e_num, opt_r, e_var);
		for(i = 0; i < size; i++) {
			for(j = 0; j < e_num; j++) {
				//e_var 배열에 저장된 순서에 따라 출력
				if(is_same_var(environ[j], e_var[i])) {
					printf("%s\n", environ[j]);
					break;
				}
			}
		}
	}

	fclose(fp);
}

//-k 옵션 설정 여부와 오류를 체크하는 함수
int check_opt_k_info(struct option_data *opt_k_info, struct option_data *opt_m_info) {
	if(opt_k_info->exist) {
		if(opt_k_info->order == opt_m_info->order + 1)
			return 1;	
		else 
			return 0;
	}
	else
		return 0;
}

//같은 변수인지 확인하는 함수
int is_same_var(char *str1, char *str2) {
	int same = 1;
	int i;
	for(i = 0; i < (int) strlen(str2); i++) {
		if(str1[i] != str2[i]) {
			same = 0;
			break;
		}
	}

	return same;
}

//qsort의 인자로 들어가는 비교함수
int compare(const void *a, const void *b ) {
	char *str_a = (char *)a;
	char *str_b = (char *)b;
	return strcmp(str_a, str_b);
}

//-k 옵션을 실행시키는 함수
int execute_option_k(struct option_data *option_info, char environ[][2048], int e_num, int opt_r, char e_var[][2048]) {	
	int i, j;
	char temp[256][2048];
	int size;

	for(i = 0; i < e_num; i++) {
		j = 0;

		//환경변수만 저장
		while(1) {
			if(environ[i][j] == '=') {
				temp[i][j] = '\0';
				break;
			}

			temp[i][j] = environ[i][j];
			j++;
		}
	}
	size = 0;
	
	if(option_info->num >= 2)//인자 최대개수 체크
		max_argument(option_info);

	//입력받은 환경변수만 e_var에 저장
	for(i = 0; i < option_info->num; i++) {
		for(j = 0; j < e_num; j++) {
			if(!strcmp(temp[j], option_info->list[i])) {
				strcpy(e_var[size], temp[j]);
				size++;
			}
		}
	}
	
	if(opt_r)//-r 옵션 설정 시 알파벳 순서로 정렬
		qsort(e_var, size, 2048, compare);
	
	return size;
}


#endif
