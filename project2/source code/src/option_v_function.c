#ifndef __OPTION_V_FUNCTION_C__
#define __OPTION_V_FUNCTION_C__

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "option_setting.h"
#include "common.h"
#include "option_v_function.h"

//-v 옵션을 실행시키는 함수
void execute_option_v(int p_path) {	
	char path[] = "/proc/version";

	if(p_path)
		printf("([%s])\n", path);
	if(check_access(path) == -1)	
		return;
	print_version_info(path);
}

//version 파일 정보를 출력해주는 함수
void print_version_info(const char *file) {
	FILE *fp = fopen(file, "r");
	char buf[1024];
	int size;
	int i;

	if(fp == NULL) {
		printf("fopen error for %s", file);
		return;
	}
	
	i = 0;
	while(fread(&buf[i], 1, 1, fp) == 1) {
		if(buf[i] == '\n') {
			buf[i] = '\0';
			break;
		}
		i++;
	}
	size = i;

	//#을 만나면 개행하고 데이터 끝에서도 개행한다.
	for(i = 0; i < size; i++) {
		if(buf[i] == '#')
			printf("\n");
		printf("%c", buf[i]);
		if(i == size - 1)
			printf("\n");
	}

	fclose(fp);
}

#endif
