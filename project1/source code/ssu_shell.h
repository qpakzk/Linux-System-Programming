#ifndef __SSU_SHELL_H__
#define __SSU_SHELL_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

//명령의 최대길이
#define N 100000

//명령을 파싱하는 함수
int parsing(char *, char *[]);
//ssu_sed 프로그램을 실행시키는 함수
void ssu_sed(int, char *[]);

#endif
