#ifndef __SSU_STRING_H__
#define __SSU_STRING_H__

#include "ssu_stdio.h"
#include "ssu_sed.h"

//<string.h>의 strlen 함수를 구현한 함수
int ssu_strlen(char *);
//<string.h>의 strcpy 함수를 구현한 함수
void ssu_strcpy(char *, char *);
//<string.h>의 strcat 함수를 구현한 함수
void ssu_strcat(char *, char *);
//<string.h>의 strcmp 함수를 구현한 함수
int ssu_strcmp(char *, char *);
//문자를 비교하는 함수
int compare_char(char, char, int);
//<string.h>의 strstr 함수를 구현한 함수
int ssu_strstr(const char *, const char *, int);
//haystack의 start와 end 인덱스 범위 내에서 needle과 같은 문자열이 있는지 검사하는 함수
int ssu_strstr2(const char *, const char *, int, int, int);
//haystack의 start와 end 인덱스 범위 내에서
//haystack의 공백을 무시하고 needle과 같은 문자열이 있는지 검사하는 함수
int ssu_strstr3(const char *, const char *, int, int, int *, int);

#endif
