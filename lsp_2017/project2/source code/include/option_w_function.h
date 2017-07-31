#ifndef __OPTION_W_FUNCTION_H__
#define __OPTION_W_FUNCTION_H__

#include "option_setting.h"

struct interrupts_info {
	char name[16];
	int num[16];
	double average;
	char description[256];
};

//-w 옵션을 실행시키는 함수
void execute_option_w(int p_path);
//interrupts 정보를 파싱하는 함수
void print_interrupts_info(const char *file);
//interrupts 정보를 조작하여 출력하는 함수
void exetrace_interrupts_info(char interrupts[][256], int int_num);

#endif
