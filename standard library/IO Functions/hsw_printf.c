#include <stdarg.h>
#include <unistd.h>
#include "hsw_string.h"
#include "hsw_stdio.h"

void hsw_itos(int n, char *buf, int base) {
	int i = 0, j;
	int dividend, quotient, remainder;
	char temp;
	if(n < 0)
		dividend = -n;
	else
		dividend = n;
	while(1) {
		remainder = dividend % base;
		quotient = dividend / base;
		buf[i] = remainder + '0';
		i++;
		if(quotient == 0)
			break;
		dividend = quotient;
	}
	
	if(n < 0) {
		buf[i] = '-';
		i++;
	}	
	buf[i] = 0;
	for(j = 0; j < i / 2; j++) {
		temp = buf[j];
   		buf[j] = buf[i - j - 1];
		buf[i - j - 1] = temp; 
	}
}

int hsw_printf(const char *format, ...) {
	int len = 0;
	const char *fp = format;
	va_list vl;
	int n;
	char number[33];
	char *np = number;
	char *str;
	char ch;

	va_start(vl, format);

	while(*fp) {
		if(*fp == '%') {
			fp++;
			if(*fp == 'd') {//decimal
				n = va_arg(vl, int);
				hsw_itos(n, number, 10);
				while(*np) {
					len += write(1, (char *)np, sizeof(char));	
					np++;
				}
			}
			else if(*fp == 's') {
				str = va_arg(vl, char *);
				len += write(1, str, hsw_strlen(str));	
			}
			else if(*fp == 'c') {
				ch = va_arg(vl, int);
				len += write(1, &ch, sizeof(char));		
			}
		}
		else
			len += write(1, fp, sizeof(char));
		fp++;
	}

	return len;
}
