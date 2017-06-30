#ifndef __OPTION_M_FUNCTION_H__
#define __OPTION_M_FUNCTION_H__

#include "option_setting.h"

//-m 옵션을 실행시키는 함수
void execute_option_m(struct option_data *opt_info, int p_path, int opt_r, struct option_data *opt_k_info);
// /proc/[PID]/environ을 생성하는 함수
void make_proc_environ(char *file, char *pid);
//environ 정보를 출력하는 함수
void print_environ_info(const char *file, struct option_data *option_k_info, int check, int opt_r); 
//-k 옵션을 실행시키는 함수
int execute_option_k(struct option_data *option_info, char environ[][2048], int e_num, int opt_r, char e_var[][2048]);
//같은 변수인지 확인하는 함수
int is_same_var(char *str1, char *str2);
//qsort의 인자로 들어가는 비교함수
int compare(const void * a, const void * b );
//-k 옵션 설정 여부와 오류를 체크하는 함수
int check_opt_k_info(struct option_data *opt_k_info, struct option_data *opt_m_info);

#endif
