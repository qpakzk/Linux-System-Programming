#ifndef __COMMON_C__
#define __COMMON_C__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

//PID나 KEY의 최대개수(16) 초과여부를 체크하는 함수
void max_argument(struct option_data *opt_info) {
	int i;
	//16을 넘으면 오류메시지를 출력하고 16으로 조정
	if(opt_info->num >= 17) {
		for(i = 16; i < opt_info->num; i++)
			printf("Maximum Number of Argument Exceeded. :: %s\n", opt_info->list[i]);
		opt_info->num = 16;
	}
}

//접근권한을 체크하는 함수
int check_access(char *path) {
	if(access(path, F_OK) == -1) {
		printf("%s doesn't exist.\n", path);
		return 0;
	}
	
	if(access(path, R_OK) == -1) {
		printf("%s can't be read.\n", path);
		return 0;
	}

	return 1;
}

//정수에서 문자열로 변환하는 함수
void trans_int_to_str(int num, char *str) {
	int a = num;
	int  i, len = 0;
	char temp;

	a = num;
	while(a) {
		str[len] = a % 10 + '0';
		a /= 10;
		len++;
	}

	str[len] = '\0';

	for(i = 0; i < len / 2; i++) {
		temp = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = temp;
	}
}

//qsort의 인자로 들어갈 비교함수
int comp(const void *n1, const void *n2) {
	if(*(int *)n1 < *(int *)n2)
		return -1;
	else if(*(int *)n1 == *(int *)n2)
		return 0;
	return 1;
}

//오름차순으로 정렬
void increasing_pid(struct option_data *opt_info) {
	int i;
	int num = opt_info->num;
	int *n_pid = (int *)malloc(sizeof(int) * num);
	char s_str[256];
	for(i = 0; i < num; i++)
		n_pid[i] = atoi(opt_info->list[i]);	
	
	qsort(n_pid, num, sizeof(int), comp);
	
	for(i = 0; i < num; i++) {
		trans_int_to_str(n_pid[i], s_str);
		strcpy(opt_info->list[i], s_str);
	}
}

#endif
