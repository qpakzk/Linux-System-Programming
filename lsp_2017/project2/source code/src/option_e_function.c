#ifndef __OPTION_E_FUNCTION_C__
#define __OPTION_E_FUNCTION_C__

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "option_setting.h"
#include "common.h"
#include "option_e_function.h"

//-e 옵션을 실행시키는 함수
void execute_option_e(int p_path) {	
	char path[] = "/proc/filesystems";

	if(p_path)
		printf("([%s])\n", path);
	if(check_access(path) == -1)	
		return;
	print_filesystems_info(path);
}

//filesystems 파일 내용을 파싱하는 함수
void print_filesystems_info(const char *file) {
	FILE *fp = fopen(file, "r");
	char filesystems[256][256];
	int i, j;
	int fs_num;

	if(fp == NULL) {
		printf("fopen error for %s\n", file);
		return;
	}

	i = 0;
	j = 0;
	//filesystems 파일 내용을 입력받으면서 줄단위로 파싱
	while(fread(&filesystems[i][j], 1, 1, fp) == 1) {
		if(filesystems[i][j] == '\n') {
			filesystems[i][j] = '\0';
			i++;
			j = 0;	
			continue;
		}
		j++;
	}
	
	fs_num = i;
	
	// '/t'를 ' '로 교체
	for(i = 0; i < fs_num; i++) {
		for(j = 0; j < (int) strlen(filesystems[i]); j++) {
			if(filesystems[i][j] =='\t')
				filesystems[i][j] = ' ';
		}
	}
	exetrace_filesystems_info(filesystems, fs_num);
	
	fclose(fp);
}

//filesystems 정보를 조작하여 출력하는 함수
void exetrace_filesystems_info(char filesystems[][256], int fs_num) {
	char result[256][256];
	char temp[256];
	int size, len;
	int i, j, k;

	size = 0;
	for(i = 0; i < fs_num; i++) {
		j = 0;
		len = 0;
		memset(temp, 0, sizeof(temp));
		while(filesystems[i][j] != ' ') {
			temp[j] = filesystems[i][j];
			j++;
		}
		temp[j] = '\0';

		//nodev가 아닐 때만 저장
		if(strcmp(temp, "nodev") != 0) {
			while(filesystems[i][j] == ' ')
				j++;
			for(k = j; k < (int) strlen(filesystems[i]); k++) {
				result[size][len] = filesystems[i][k];
				len++;
			}
			
			result[size][len] = '\0';
			size++;
		}
	}
	
	//명세서에서 요구하는 방식대로 출력
	printf("<Supported Filesystems>\n");
	for(i = 0; i < size; i++) {
		if(i % 5 == 0 && i != 0) {
			if(i == size - 1)
				printf("\n%s\n", result[i]);
			else
				printf("\n%s, ", result[i]);
		}
		else {
			if(i == size - 1)
				printf("%s\n", result[i]);
			else
				printf("%s, ", result[i]);
		}
	}
}

#endif
