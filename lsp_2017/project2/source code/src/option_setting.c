#ifndef __OPTION_SETTING_C__
#define __OPTION_SETTING_C__

#include <stdio.h>
#include <string.h>
#include "option_setting.h"
#include "execution.h"

//옵션을 설정하는 함수
int set_option(int argc, char *argv[], struct option_data *opt_info) {
	int i;
	int opt;
	int order = 1;
	int opt_num = 0;

	if(argc > 1 && argv[1][0] != '-') {//오류체크
		printf("ERROR: usage: %s [OPTION]\n", argv[0]);
		return -1;
	}
	
	//옵션 설정
	//옵션 정보를 opt_info 구조체에 저장
	for(i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			if(!strcmp(argv[i], "-f")) {
				opt = _F;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else if(!strcmp(argv[i], "-t")) {
				opt = _T;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else if(!strcmp(argv[i], "-c")) {
				opt = _C;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else if(!strcmp(argv[i], "-n")) {
				opt = _N;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else if(!strcmp(argv[i], "-m")) {
				opt = _M;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else if(!strcmp(argv[i], "-k")) {
				opt = _K;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else if(!strcmp(argv[i], "-w")) {
				opt = _W;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else if(!strcmp(argv[i], "-e")) {
				opt = _E;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else if(!strcmp(argv[i], "-l")) {
				opt = _L;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else if(!strcmp(argv[i], "-v")) {
				opt = _V;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else if(!strcmp(argv[i], "-r")) {
				opt = _R;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else if(!strcmp(argv[i], "-s")) {
				opt = _S;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else if(!strcmp(argv[i], "-o")) {
				opt = _O;
				opt_info[opt].exist++;
				opt_info[opt].order = order;
				order++;
				opt_num++;
			}
			else {
				printf("ERROR: There is no %s option.\n", argv[i]);
				return -1;
			}	
		}
		else {
			strcpy(opt_info[opt].list[opt_info[opt].num], argv[i]);
			opt_info[opt].num++;
		}
	}
	
	for(i = 0; i < 13; i++) {//동일 옵션 중복 사용 체크
		if(opt_info[i].exist >= 2) {
			printf("ERROR: It is impossible to use same option more than twice.\n");
			return -1;
		}
	}

	if(opt_info[_K].exist) {//-k 옵션이 -m 옵션 바로 뒤에 나오는지 체크
		if(opt_info[_K].order != opt_info[_M].order + 1) {
			printf("ERROR: usage: %s -m [PID1][PID2]..[PID16] [-k] [KEY1][KEY2]..[KEY16]\n", argv[0]);
			return -1;
		}
	}
	
	if(opt_info[_S].exist) {//-s 옵션 오류 체크
		if(opt_info[_S].num <= 0 || opt_info[_S].num >= 6) {
			printf("ERROR: usage: %s -s [ATTRIBUTE_NAME]\n", argv[0]);
			printf("[ATTRIBUTE_NAME] : FILEDES | CMDLINE | IO | STAT | ENVIRON\n");
			return -1;
		}
	}
	
	
	if(opt_info[_O].exist) {//-o 옵션 오류 체크
		if(opt_info[_O].num != 1) {
			printf("ERROR: usage %s -o [FILE_NAME]\n", argv[0]);
			return -1;
		}
	}

	
	return opt_num;
}

//path의 출력 여부를 확인하는 함수
int should_print_path(struct option_data *opt_info, int opt_num) {			
	int check = opt_num;

	if(opt_info[_O].exist)
		check--;
	
	if(opt_info[_R].exist)
		check--;

	if(opt_info[_K].exist)
		check--;
		
	//-o, -r, -k 제외를 제외한 옵션이 두 개 이상
	if(check >= 2)
		return 1;
	
	if(opt_info[_F].num >= 2)
		check += (opt_info[_F].num - 1);
	
	if(opt_info[_T].num >= 2)
		check += (opt_info[_T].num - 1);

	if(opt_info[_C].num >= 2)
		check += (opt_info[_C].num - 1);
	
	if(opt_info[_N].num >= 2)
		check += (opt_info[_N].num - 1);
	
	if(opt_info[_M].num >= 2)
		check += (opt_info[_M].num - 1);

	//PID 또는 KEY가 2개 이상
	if(check >= 2)
		return 1;
	else
		return 0; 
}

#endif
