#ifndef __OPTION_E_FUNCTION_H__
#define __OPTION_E_FUNCTION_H__

#include "option_setting.h"

//-e 옵션을 실행시키는 함수
void execute_option_e(int p_path);
//filesystems 파일 내용을 파싱하는 함수
void print_filesystems_info(const char *file);
//filesystems 정보를 조작하여 출력하는 함수
void exetrace_filesystems_info(char filesystems[][256], int fs_num);

#endif
