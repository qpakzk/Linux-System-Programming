#include "ssu_stdio.h"

//<stdio.h>의 printf 함수의 일부 기능을 구현한 함수
int ssu_printf(char *fmt, ...) {
	va_list ap;//가변인자를 위해 필요
	int i, j;
	char ch;
	char *str;
	int n;
	int num_digit;
	char digits[100];
	int len = 0;
	va_start(ap, fmt);//fmt 다음 인자부터 ap가 가리킴

	for(i = 0; i < ssu_strlen(fmt); i++) {
		ch = fmt[i];
        //문자열 변환명세 구현
        //%s, %d 만났을 경우 뒤의 인자로 치환
		if(ch == '%') {
			ch = fmt[i + 1];		
			i++;
			if(ch == 's') {
				str = va_arg(ap, char *);	
				write(1, str, ssu_strlen(str));
				len += ssu_strlen(str);
			}
			else if(ch == 'd') {//정수 변환명세 구현
				n = va_arg(ap, int);

				num_digit = 0;
				while(1) {
					digits[num_digit] = n % 10 + '0';
					n /= 10;
					num_digit++;
					if(n == 0)
						break;
				}		

				for(j = num_digit - 1; j >= 0; j--) {
					write(1, (char *)&digits[j], sizeof(char));
					len += sizeof(char);
				}
			}
		}
		else {//변환명세 이외 write()로 출력
			write(1, (char *)&ch, sizeof(char));
			len += sizeof(char);
		}
	}
    //출력하는 문자열 길이 반환
	return len;
}

//<stdio.h>의 sprintf 함수의 일부 기능을 구현한 함수
int ssu_sprintf(char *buffer, char *fmt, ...) {
	va_list ap;
	int i, j;
	char ch;
	char *str;
	int offset = 0;
	int num_digit;
	char digits[100];
    
    //%s 변환명세를 치환해 가면서
    //buffer에 fmt 문자열을 저장
	va_start(ap, fmt);
	for(i = 0; i < ssu_strlen(fmt); i++) {
		ch = fmt[i];
		if(ch == '%') {
			ch = fmt[i + 1];		
			i++;
			if(ch == 's') {//문자열 변환명세 구현
				str = va_arg(ap, char *);	
                
				for(j = 0; j < ssu_strlen(str); j++)
					buffer[offset + j] = str[j];
				offset += ssu_strlen(str);
			}
		}
		else {
			buffer[offset] = ch;
			offset++;
		}
	}

	buffer[offset] = 0;
    //buffer 문자열 길이 반환
	return offset;
}
