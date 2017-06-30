#ifndef __OPTION_L_FUNCTION_C__
#define __OPTION_L_FUNCTION_C__

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "option_setting.h"
#include "common.h"
#include "option_l_function.h"

//-l 옵션을 실행시키는 함수
void execute_option_l(int p_path) {	
	char path[] = "/proc/uptime";

	if(p_path)
		printf("([%s])\n", path);
	if(check_access(path) == -1)	
		return;
	print_uptime_info(path);
}

//uptime 정보를 파싱하는 함수
void print_uptime_info(const char *file) {
	FILE *fp = fopen(file, "r");
	char buf[1024];
	int size;
	char worked_time[32];
	char idle_time[32];
	int i, j, k;

	if(fp == NULL) {
		printf("fopen error for %s", file);
		return;
	}
	
	i = 0;
	//buf에 uptime 정보 
	while(fread(&buf[i], 1, 1, fp) == 1) {
		if(buf[i] == '\n') {
			buf[i] = '\0';
			break;
		}
		i++;
	}
	size = i;

	i = 0;

	//worked time
	while(buf[i] != ' ') {
		worked_time[i] = buf[i];				
		i++;
	}
	worked_time[i] = '\0';
	printf("Process worked time : %s(sec)\n", worked_time);	

	//idle time;
	while(buf[i] == ' ' || buf[i] == '\t')
		i++;
	k = 0;	
	for(j = i; j < size; j++) {
		idle_time[k] = buf[j];	
		k++;
	}
	idle_time[k] = '\0';	

	printf("Process idle time : %s(sec)\n", idle_time);
	fclose(fp);
}

#endif
