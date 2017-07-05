#ifndef __SSU_STDIO_H__
#define __SSU_STDIO_H__

#include <stdarg.h>
#include <unistd.h>
#include "ssu_string.h"

//<stdio.h>의 printf 함수의 일부 기능을 구현한 함수
int ssu_printf(char *fmt, ...);
//<stdio.h>의 sprintf 함수의 일부 기능을 구현한 함수
int ssu_sprintf(char *buffer, char *fmt, ...);


#endif
