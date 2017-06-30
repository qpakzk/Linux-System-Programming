#ifndef __OPTION_W_FUNCTION_C__
#define __OPTION_W_FUNCTION_C__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "option_setting.h"
#include "common.h"
#include "option_w_function.h"

//-w 옵션을 실행시키는 함수
void execute_option_w(int p_path) {	
	char path[] = "/proc/interrupts";

	if(p_path)
		printf("([%s])\n", path);
	if(check_access(path) == -1)	
		return;
	print_interrupts_info(path);
}

//interrupts 정보를 파싱하는 함수
void print_interrupts_info(const char *file) {
	FILE *fp = fopen(file, "r");
	char interrupts[256][256];
	int i, j;
	int int_num;
	if(fp == NULL) {
		printf("fopen error for %s\n", file);
		return;
	}

	i = 0;
	j = 0;
	
	//interrupts 배열에 줄단위로 파싱하여 저장
	while(fread(&interrupts[i][j], 1, 1, fp) == 1) {
		if(interrupts[i][j] == '\n') {
			interrupts[i][j] = '\0';
			i++;
			j = 0;	
			continue;
		}
		j++;
	}
	
	int_num = i;
	
	exetrace_interrupts_info(interrupts, int_num);
	fclose(fp);
}

//interrupts 정보를 조작하여 출력하는 함수
void exetrace_interrupts_info(char interrupts[][256], int int_num) {
	struct interrupts_info result[256] = {0};
	int size, len;
	int i, j, k, m, n;
	char cpu;
	int cpu_num;
	char temp[32] = {0};
	int sum;

	//마지막 cpu번호를 통해 cpu 개수 알아내기
	for(i = (int) strlen(interrupts[0]) - 1 ; i >= 0; i--)
		if(interrupts[0][i] != ' ')	{
			cpu = interrupts[0][i];
			break;
		}
	cpu_num = cpu -'0' + 1;

	printf("---Number of CPUs : %d---\n", cpu_num);

	size = 0;
	//interrupts 배열에 저장된 정보를 파싱하여 result 배열에 저장
	for(i = 1; i < int_num; i++) {
		len = (int) strlen(interrupts[i]); 
		j = 0;
		while(interrupts[i][j] == ' ' && j < len)
			j++;

		//숫자로 시작하는 것들 제끼기
		if(isdigit(interrupts[i][j]))
			continue;
		k = 0;
		while(interrupts[i][j] != ':' && j < len) {
			result[size].name[k] = interrupts[i][j];
			k++;
			j++;
		}
		result[size].name[k] = '\0';
		
		j++;	
		k = 0;
		while(1) {
			if(interrupts[i][j] != ' ' && j < len) {
				m = 0;

				//cpu 정보 저장
				while(interrupts[i][j] != ' ' && j < len) {
					temp[m] = interrupts[i][j];
					m++;
					j++;
				}
				temp[m] = '\0';
				result[size].num[k] = atoi(temp);
				k++;

				if(k == cpu_num) {//cpu 개수로 cpu 항목값들 평균내기
					sum = 0;
					for(n = 0; n < k; n++)
						sum += result[size].num[n];
					result[size].average = (double) sum / k;
					break;
				}

				if(j == len && k < cpu_num) {//cpu 항목값이  cpu 개수 보타 적은 경우 체크
					sum = 0;
					for(n = 0; n < k; n++)
						sum += result[size].num[n];
					result[size].average = (double) sum / k;	
					break;
				}
				continue;
			}
			j++;
		}

		while(interrupts[i][j] == ' ' && j < len)
			j++;
		
		//설명 저장
		k = 0;
		while(j < len) {
			result[size].description[k] = interrupts[i][j];
			k++;
			j++;
		}
		
		result[size].description[k] = '\0';
		size++;
	}
	
	//명세서의 요구에 맞는 형식으로 출력
	printf("      [Average] : [Description]\n");
	for(i = 0; i < size; i++) {
		printf("%15.3lf : <%s> %s\n", result[i].average, result[i].name, result[i].description);
	}
}

#endif
