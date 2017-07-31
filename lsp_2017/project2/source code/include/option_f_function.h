#ifndef __OPTION_F_FUNCTION_H__
#define __OPTION_F_FUNCTION_H__

#include "option_setting.h"

//-f 옵션을 실행시키는 함수
void execute_option_f(struct option_data *opt_info, int p_path, int opt_r);
// /proc/[PID]/fd 디렉터리를 생성하는 함수
void make_proc_fd_dir(char *dir, char *pid);
// /proc/[PID]/fd/[fd number] 파일을 생성하는 함수
void make_proc_fd_file(char *dir, char *pid, char *fname);
//fd 정보를 출력하는 함수
void search_n_print_fd_info(char *dir, char *pid);

#endif
