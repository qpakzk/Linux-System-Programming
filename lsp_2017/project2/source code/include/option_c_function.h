#ifndef __OPTION_C_FUNCTION_H__
#define __OPTION_C_FUNCTION_H__

#include "option_setting.h"

//-c 옵션을 실행시키는 함수
void execute_option_c(struct option_data *opt_info, int p_path, int opt_r);
// /proc/[PID]/cmdline을 생성하는 함수
void make_proc_cmdline(char *file, char *pid);
//cmdline 정보를 출력하는 함수
void print_cmdline_info(const char *file);

#endif
