#ifndef __OPTION_N_FUNCTION_H__
#define __OPTION_N_FUNCTION_H__

#include "option_setting.h"

//-n 옵션을 실행시키는 함수
void execute_option_n(struct option_data *opt_info, int p_path, int opt_r);
// /proc/[PID]/io를 생성하는 함수
void make_proc_io(char *file, char *pid);
//io 정볼를 출력하는 함수
void print_io_info(const char *file);
//명세서에 요구하는 형식으로 변경하는 함수
void io_arrangement(char io[][128]);

#endif
