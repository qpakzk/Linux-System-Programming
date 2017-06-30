#ifndef __OPTION_S_FUNCTION_H__
#define __OPTION_S_FUNCTION_H__

#include "option_setting.h"

#define FILEDES 0
#define CMDLINE 1
#define IO 2
#define STAT 3
#define ENVIRON 4

//-s 옵션을 실행시키는 함수
void execute_option_s(struct option_data *opt_info, int p_path);
//-s 옵션의 FILEDES 인자를 실행하는 함수
void execute_option_s_FILEDES(char *pid, int p_path);
//-s 옵션의 CMDLINE 인자를 실행하는 함수
void execute_option_s_CMDLINE(char *pid, int p_path);
//-s 옵션의 IO 인자를 실행하는 함수
void execute_option_s_IO(char *pid, int p_path);
//-s 옵션의 STAT 인자를 실행하는 함수
void execute_option_s_STAT(char *pid, int p_path);
//-s 옵션의 ENVIRON 인자를 실행하는 함수
void execute_option_s_ENVIRON(char *pid, int p_path);
//uid와 일치하는 pid를 추출하는 함수
int extract_pid(char *uid, char pid_list[][128]);

#endif
