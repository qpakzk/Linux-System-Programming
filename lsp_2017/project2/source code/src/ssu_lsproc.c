#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "option_setting.h"
#include "execution.h"

int main(int argc, char *argv[]) {
	FILE *fout = tmpfile(), *fp;
	int out = fileno(fout), fd, opt_num, check; 
	struct option_data opt_info[13] = {0};

	dup2(1, out);//out에 표준 출력 파일 디스크립터 복제 
	fprintf(fout, ">: ssu_lsproc start. :<\n");

	//opt_info에 옵션 별로 분류하여 저장
	opt_num = set_option(argc, argv, opt_info); 

	//(1)하나의 옵션에 두 개 이상의 PID가 설정된 경우 check == 1
	//(2)두 개의 옵션이 설정된 경우 check == 1
	//그 이외에는 check == 0
	if(opt_num != -1) {
		check = should_print_path(opt_info, opt_num);

		//-o 옵션 설정 여부 확인
		if(opt_info[_O].exist) {
			//redirection할 파일을 쓰기 모드로 open
			fp = fopen(opt_info[_O].list[0], "w");
			fd = fileno(fp);
			dup2(fd, 1);//1번 파일디스크립터를 fd로 복제
			printf("!--Successfully Redirected : %s--!\n", opt_info[_O].list[0]);
		}

		//옵션 실행
		execute(opt_info, check);
	}
	//-o 옵션이 설정되더라도 표준 출력 가능
	fprintf(fout, ">: ssu_lsproc terminated. :<\n");
	exit(0);	
}
