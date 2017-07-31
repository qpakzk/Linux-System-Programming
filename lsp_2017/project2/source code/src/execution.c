#ifndef __EXECUTION_C__
#define __EXECUTION_C__

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "execution.h"
#include "option_f_function.h"
#include "option_t_function.h"
#include "option_c_function.h"
#include "option_n_function.h"
#include "option_m_function.h"
#include "option_w_function.h"
#include "option_e_function.h"
#include "option_l_function.h"
#include "option_v_function.h"
#include "option_s_function.h"

//전반적인 실행을 관리하는 함수
void execute(struct option_data *opt_info, int p_path) {
	int i;
	pid_t pid;
	int opt_r = 0;

	//-r 옵션 체크
	if(opt_info[_R].exist)
		opt_r = 1;
	
	for(i = 0; i < 13; i++) {
		//-r, -o 옵션은 자식 프로세스 생성 X
		//-k 옵션은 -m 옵션의 종속 옵션이므로 단독으로 자식 프로세스 생성 X
		if(i == _K || i == _R || i == _O)
			continue;

		//옵션이 존재하면
		if(opt_info[i].exist) {
			pid = fork();//자식 프로세스 생성
			if(pid == 0) {//자식 프로세스가 옵션을 실행
				execute_option(&opt_info[i], i, p_path, opt_r, &opt_info[_K]);
				exit(0);//자식 프로세스의 수행이 끝나면 종료
			}
			else if(pid > 0)//부모 프로세스는 자식 프로세스가 종료될 때까지 대기
				waitpid(pid, (int *)0, 0);
		}
	}
}

//옵션을 실행시키는 함수
void execute_option(struct option_data *opt_info, int opt, int p_path, int opt_r, struct option_data *opt_k_info) {
	//옵션 별로 지정된 함수 실행
	if(opt == _F)
		execute_option_f(opt_info, p_path, opt_r);
	else if(opt == _T)
		execute_option_t(opt_info, p_path, opt_r);
	else if(opt == _C) 
		execute_option_c(opt_info, p_path, opt_r);
	else if(opt == _N)
		execute_option_n(opt_info, p_path, opt_r);
	else if(opt == _M)
		execute_option_m(opt_info, p_path, opt_r, opt_k_info);
	else if(opt == _W)
		execute_option_w(p_path);
	else if(opt == _E)
		execute_option_e(p_path);
	else if(opt == _L)
		execute_option_l(p_path);
	else if(opt == _V)
		execute_option_v(p_path);
	else if(opt == _S)
		execute_option_s(opt_info, p_path);
}

#endif
