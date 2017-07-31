#ifndef __OPTION_T_FUNCTION_H__
#define __OPTION_T_FUNCTION_H__

#include "option_setting.h"

//-t 옵션을 실행시키는 함수
void execute_option_t(struct option_data *opt_info, int p_path, int opt_r);
// /proc/[PID]/status 파일을 생성하는 함수
void make_proc_status(char *file, char *pid);
//status 정보를 출력하는 함수
void print_status_info(const char *file);
//명세서의 요구조건에 맞게 변형하는 함수
void status_arrangement(char status[][128]);

#endif
