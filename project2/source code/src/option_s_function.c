#ifndef __OPTION_S_FUNCTION_C__
#define __OPTION_S_FUNCTION_C__

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include "option_setting.h"
#include "common.h"
#include "option_s_function.h"
#include "option_f_function.h"
#include "option_c_function.h"
#include "option_n_function.h"
#include "option_t_function.h"
#include "option_m_function.h"

//-s 옵션을 실행시키는 함수
void execute_option_s(struct option_data *opt_info, int p_path) {	
	int i;
	uid_t uid = getuid();
	char str_uid[256];
	int argv_table[5] = {0};
	int ls_num;
	char pid_list[1024][128];

	if(opt_info->num == 0) {
		printf("-s option error\n");
		return;
	}

	//입력받는 인자 설정
	for(i = 0; i < opt_info->num; i++) {
		if(!strcmp(opt_info->list[i], "FILEDES"))
			argv_table[FILEDES]++;
		else if(!strcmp(opt_info->list[i], "CMDLINE")) 
			argv_table[CMDLINE]++;
		else if(!strcmp(opt_info->list[i], "IO")) 
			argv_table[IO]++;
		else if(!strcmp(opt_info->list[i], "STAT")) 
			argv_table[STAT]++;
		else if(!strcmp(opt_info->list[i], "ENVIRON")) 
			argv_table[ENVIRON]++;
		else
			printf("error\n");
	}
	
	for(i = 0; i < opt_info->num; i++) {//중복체크
		if(argv_table[i] >= 2) {
			printf("It is impossible to use same arguments of -s option more than twice.\n");
			return;
		}
	}


	trans_int_to_str(uid, str_uid);
	ls_num = extract_pid(str_uid, pid_list);

	//해당 인자가 설정되었을 때 실행
	for(i = 0; i < ls_num; i++) {
		if(argv_table[FILEDES])
			execute_option_s_FILEDES(pid_list[i], p_path);
		if(argv_table[CMDLINE])
			execute_option_s_CMDLINE(pid_list[i], p_path);
		if(argv_table[IO])
			execute_option_s_IO(pid_list[i], p_path);
		if(argv_table[STAT])
			execute_option_s_STAT(pid_list[i], p_path);
		if(argv_table[ENVIRON])
			execute_option_s_ENVIRON(pid_list[i], p_path);
	}
}

//-s 옵션의 FILEDES 인자를 실행하는 함수
void execute_option_s_FILEDES(char *pid, int p_path) {
	char path[1024];
	
	snprintf(path, 1024, "/proc/%s/fd", pid);

	if(p_path)
		printf("([%s])\n", path);

	// /proc/[PID]/fd 디렉터리에 접근할 수 있는지 체크
	if(access(path, X_OK) == 0) {
		printf("## Attribute : FILEDES, Target Process ID : %s ##\n", pid);
		search_n_print_fd_info(path, pid);
	}
}

//-s 옵션의 CMDLINE 인자를 실행하는 함수
void execute_option_s_CMDLINE(char *pid, int p_path) {
	char path[1024];
		
	snprintf(path, 1024, "/proc/%s/cmdline", pid);

	if(p_path)
		printf("([%s])\n", path);

	if(access(path, R_OK) == 0) {
		printf("## Attribute : CMDLINE, Target Process ID : %s ##\n", pid);
		print_cmdline_info(path);
	}
}

//-s 옵션의 IO 인자를 실행하는 함수
void execute_option_s_IO(char *pid, int p_path) {
	char path[1024];

	snprintf(path, 1024, "/proc/%s/io", pid);
	
	if(p_path)
		printf("([%s])\n", path);
	
	if(access(path, R_OK) == 0) {
		printf("## Attribute : IO, Target Process ID : %s ##\n", pid);
		print_io_info(path);
	}

}

//-s 옵션의 STAT 인자를 실행하는 함수
void execute_option_s_STAT(char *pid, int p_path) {
	char path[1024];
		
	snprintf(path, 1024, "/proc/%s/status", pid);

	if(p_path)
		printf("([%s])\n", path);

	if(access(path, R_OK) == 0) {
		printf("## Attribute : STAT, Target Process ID : %s ##\n", pid);
		print_status_info(path);
	}
}

//-s 옵션의 ENVIRON 인자를 실행하는 함수
void execute_option_s_ENVIRON(char *pid, int p_path) {
	char path[1024];
		
	snprintf(path, 1024, "/proc/%s/environ", pid);

	if(p_path)
		printf("([%s])\n", path);

	if(access(path, R_OK) == 0) {
		printf("## Attribute : ENVIRON, Target Process ID : %s ##\n", pid);
		print_environ_info(path, NULL, 0, 0);
	}

}

//uid와 일치하는 pid를 추출하는 함수
int extract_pid(char *uid, char pid_list[][128]) {
	DIR *dp;
	char path[1024];
	struct dirent *dent;
	FILE *fp;
	char buf[2048], *pb, id[32];
	int fsize;
	int i, j, ls_num = 0;

	dp = opendir("/proc");// /proc 디렉테리 open
	while((dent = readdir(dp)) != NULL) {// /proc 디렉터리 read
		if(!isdigit(*dent->d_name))//pid가 아니면 제끼기
			continue;
		
		// status 파일에서 uid 확인
		snprintf(path, 1024, "/proc/%s/status", dent->d_name);
		
		fp = fopen(path, "r");
		if(fp == NULL)
			continue;

		i = 0;
		//buf에 status 정보를 저장
		while(fread(&buf[i], 1, 1, fp) == 1)
			i++;
		fsize = i;
		buf[fsize] = '\0';

		//Uid를 찾아서 uid 번호를 알아내기
		pb = strstr(buf, "Uid");	
		memset(id, 0, sizeof(id));
		j = 0;
		while(1) {
			if(isdigit(*pb)) {
				id[j] = *pb;
				j++;
				pb++;

				while(isdigit(*pb)) {
					id[j] = *pb;
					j++;
					pb++;
				}
				id[j] = '\0';
				break;
			}
			pb++;	
		}
	
		//uid가 동일하면 pid를 pid_list 배열에 저장
		if(!strcmp(id, uid)) {
			strcpy(pid_list[ls_num], dent->d_name);
			ls_num++;
		}
	}

	closedir(dp);
	return ls_num;
}

#endif
