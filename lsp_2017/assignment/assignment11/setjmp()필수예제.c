#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>

void ssu_nested_func(int loc_var, int loc_volatile, int loc_register);

static jmp_buf glob_buffer;

int main(void) {
	//최적화 시 레지스터에 저장
	register int loc_register;
	volatile int loc_volatile;
	//최적화 시 레지스터에 저장
	int loc_var;

	loc_var = 10;
	loc_volatile = 11;
	loc_register = 12;

	//longjmp()에 의해 호출되었을 경우
	if(setjmp(glob_buffer) != 0) {
		printf("after longjmp, loc_var = %d, loc_volatile = %d, loc_register = %d\n", loc_var, loc_volatile, loc_register);
		exit(0);
	}

	loc_var = 80;
	loc_volatile = 81;
	loc_register = 83;
	ssu_nested_func(loc_var, loc_volatile, loc_register);
	exit(0);
}

void ssu_nested_func(int loc_var, int loc_volatile, int loc_register) {
	printf("before longjmp, loc_var = %d, loc_volatile = %d, loc_register = %d\n", loc_var, loc_volatile, loc_register);
	//setjmp()로 이동, 호출된 setjmp()는 1을 반환
	longjmp(glob_buffer, 1);
}
