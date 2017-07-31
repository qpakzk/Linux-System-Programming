#ifndef __COMMON_H__
#define __COMMON_H__
	
#include "option_setting.h"

//PID나 KEY의 최대개수(16) 초과여부를 체크하는 함수
void max_argument(struct option_data *opt_info);
//접근권한을 체크하는 함수
int check_access(char *path);
//정수를 정수에서 문자열로 변환하는 함수
void trans_int_to_str(int num, char *str);
//qsort의 인자로 들어갈 비교함수
int comp(const void *n1, const void *n2);
//오름차순으로 정렬
void increasing_pid(struct option_data *opt_info);

#endif
