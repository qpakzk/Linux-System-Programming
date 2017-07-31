#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

//ssu_show()를 ssu_show_limit()으로 정의, #limit은 limit을 문자열로 바꾼다.
#define ssu_show(limit) ssu_show_limit(limit, #limit)

void ssu_show_limit(int rlim_type, char *rlim_name);

int main(void) {
	printf("	resource   soft limit   hard limit	\n\n");
	//프로세스가 소비할 수 있는 최대 CPU 시간 조회
	ssu_show(RLIMIT_CPU);
	//파일 생성 시 파일의 최대크기 조회
	ssu_show(RLIMIT_FSIZE);
	//자료 구역 전체의 최대크기 조회
	ssu_show(RLIMIT_DATA);
	//스택 최대크기 조회
	ssu_show(RLIMIT_STACK);
	//한 코어 파일의 최대크기 조회
	ssu_show(RLIMIT_CORE);

//Resident Set Size(RSS)의 최댓값 조회
#ifdef RLIMIT_RSS
	ssu_show(RLIMIT_RSS);
#endif

//한 프로세스가 mlock으로 잠글 수 있는 메모리의 최대 용량 조회
#ifdef RLIMIT_MEMLOCK
	ssu_show(RLIMIT_MEMLOCK);
#endif

//실제 사용자 ID당 최대 자식 프로세스 개수 조회
#ifdef RLIMIT_NPROC
	ssu_show(RLIMIT_NPROC);
#endif

//한 프로세스가 열어둘 수 있는 최대 파일 개수 조회
#ifdef RLIMIT_OFILE
	ssu_show(RLIMIT_OFILE);
#endif

//한 프로세스가 열어둘 수 있는 최대 파일 개수 조회
#ifdef RLIMIT_NOFILE
	ssu_show(RLIMIT_NOFILE);
#endif

//한 프로세스의 총 가용 메모리 용량의 최댓값 조회
#ifdef RLIMIT_VMEN
	ssu_show(RLIMIT_VMEN);
#endif

	exit(0);
}

//프로세스마다 적용된 자원의 한계값 조회
void ssu_show_limit(int rlim_type, char *rlim_name) {
	struct rlimit rlim;
	char cur[11], max[11];

	//프로세스마다 적용된 자원의 한계값을 rlim 구조체 변수에 저장
	getrlimit(rlim_type, &rlim);

	//soft limit 값
	if(rlim.rlim_cur == RLIM_INFINITY)
		strcpy(cur, "infinity");
	else
		sprintf(cur, "%10ld", rlim.rlim_cur);

	//hard limit 값
	if(rlim.rlim_max == RLIM_INFINITY) 
		strcpy(max, "infinity");
	else
		sprintf(max, "%10ld", rlim.rlim_max);

	//soft limit, hard limit 값 출력
	printf("%15s : %10s %10s\n", rlim_name, cur, max);
}


