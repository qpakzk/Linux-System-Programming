#ifndef __OPTION_SETTING_H__
#define __OPTION_SETTING_H__

#define _F 0
#define _T 1
#define _C 2
#define _N 3
#define _M 4
#define _K 5
#define _W 6
#define _E 7
#define _L 8
#define _V 9
#define _R 10
#define _S 11
#define _O 12

//옵션정보를 저장하는 구조체
struct option_data {
	int exist;//존재 여부 체크, 옵션 중복사용 체크
	int order;//몇 번째 옵션인지 체크, -m과 -k 옵션 관계 파악 시 사용
	int num;//옵션 인자의 개수 
	char list[256][256];//옵션 인자를 입력
};

//옵션을 설정하는 함수
int set_option(int argc, char *argv[], struct option_data *opt_info);
//path의 출력 여부를 확인하는 함수
int should_print_path(struct option_data *opt_info, int opt_num);

#endif
