#ifndef __EXECUTION_H__
#define __EXECUTION_H__

#include "option_setting.h"

//전반적인 실행을 관리하는 함수
void execute(struct option_data *opt_info, int opt_num);
//옵션을 실행시키는 함수
void execute_option(struct option_data *opt_info, int opt, int p_path, int opt_r, struct option_data *opt_k_info);

#endif
