#ifndef __SSU_SED_H__
#define __SSU_SED_H__

#include "ssu_string.h"
#include "ssu_stdio.h"
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <stdlib.h>

#define N 100000

//디렉터리를 탐색하고 파일을 처리하는 함수
void search_dir(char *, char *, char *, int *, char *[], int);
//파일 내에서 치환하고자 하는 문자열을 찾아 원하는 문자열로 치환하는 함수
int check_n_change(char *,char *, char *, char *, int *);
//파일 내에서 치환된 문자열의 라인을 찾아주는 함수
int find_line(char *, int);
//usage 출력함수
void print_usage(char *);
#endif
