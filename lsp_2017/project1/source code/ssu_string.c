#include "ssu_string.h"

//<string.h>의 strlen 함수를 구현한 함수
int ssu_strlen(char *str) {
	int len = 0;
	while(str[len] != '\0')
		len++;
	return len;
}

//<string.h>의 strcpy 함수를 구현한 함수
void ssu_strcpy(char *dest, char *src) {
	int i = 0;
	while(src[i] != '\0') {
		dest[i] = src[i];
		i++;
	}	
	dest[i] = '\0';
}

//<string.h>의 strcat 함수를 구현한 함수
void ssu_strcat(char *src1, char *src2) {
	int len1 = ssu_strlen(src1);
	int len2 = ssu_strlen(src2);
	int i;
	for(int i = 0; i < len2; i++)
		src1[i + len1] = src2[i];
	src1[len1 + len2] = '\0';
}

//<string.h>의 strcmp 함수를 구현한 함수
int ssu_strcmp(char *src1, char *src2) {
	int len1 = ssu_strlen(src1);
	int len2 = ssu_strlen(src2);
	int i;
	int isSame = 1;
	if(len1 != len2)
		return -1;
	for(i = 0; i < len1; i++)
		if(src1[i] != src2[i]) {
			isSame = 0;
			break;
		}

	if(isSame)
		return 0;
	else
		return -1;
}

//문자를 비교하는 함수
int compare_char(char c1, char c2, int no_upper_lower) {
    //1인 경우 대소문자 구분 안함
    if(no_upper_lower) {
        //대문자라면 소문자로 바꿈
		if(c1 >= 65 && c1 <= 90) 
				c1 += 32;
		if(c2 >= 65 && c2 <= 90)
				c2 += 32;	
	}

	if(c1 == c2)
		return 1;
	else
		return 0;
	

}

//<string.h>의 strstr 함수를 구현한 함수
int ssu_strstr(const char *haystack, const char *needle, int no_upper_lower) {
	int i = 0, j = 0;
	int same = 0;
	if(*needle == '\0')
		return 1; 

	while(haystack[i] != '\0') {
		while(compare_char(haystack[i], needle[j], no_upper_lower)) {
			same++;
			if(same == ssu_strlen((char *)needle))
				return 1;
			i++;
			j++;
		}	
		if(compare_char(haystack[i], needle[j], no_upper_lower) == 0) {
			j = 0;
			same = 0;
		}
		i++;
	}

	return 0;
}

//haystack의 start와 end 인덱스 범위 내에서 needle과 같은 문자열이 있는지 검사하는 함수
int ssu_strstr2(const char *haystack, const char *needle, int start, int end, int no_upper_lower) {
	int i = start, j = 0;
	int same = 0;
	int first = -1;;
	if(*needle == '\0')
		return start; 

	while(haystack[i] != '\0' && i <= end) {
		while(compare_char(haystack[i], needle[j], no_upper_lower) && i <= end) {
			if(first == -1)//haystack에서 needle과 공통 문자열의 첫번 째 인덱스 저장
				first = i;	
			same++;
			if(same == ssu_strlen((char *)needle))
				return first;
			i++;
			j++;
		}	
		if(compare_char(haystack[i], needle[j], no_upper_lower) == 0 && i <= end) {
			first = -1;
			j = 0;
			same = 0;
		}
		i++;
	}

	return -1;
}

//haystack의 start와 end 인덱스 범위 내에서
//haystack의 공백을 무시하고 needle과 같은 문자열이 있는지 검사하는 함수
int ssu_strstr3(const char *haystack, const char *needle, int start, int end, int *empty, int no_upper_lower) {
	int i = start, j = 0;
	int same = 0;
	int first = -1;
	if(*needle == '\0')
		return start; 

	while(haystack[i] != '\0' && i <= end) {
		if(compare_char(haystack[i], needle[j], no_upper_lower)) {
			if(first == -1)
				first = i;
			i++;
			j++;
			same++;

			if(same == ssu_strlen((char *)needle))
				return first;
			while(i <= end) {
				if(compare_char(haystack[i], needle[j], no_upper_lower) == 0) {
					if(haystack[i] == ' ') {//공백일 때
						i++;//haystack의 인덱스만 증가
						*empty += 1;//empty 개수 증가
					}
					else {
						first = -1;
						j = 0;
						same = 0;
						break;
					}
				}
				else {
					i++;
					j++;
					same++;

					if(same == ssu_strlen((char *)needle))
						return first;
				}
			}
		}

		i++;
	}

	return -1;
}
