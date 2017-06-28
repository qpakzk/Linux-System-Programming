#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(void) {
	char operator;
	FILE *fp;
	int character;
	int number = 0;

	//ssu_expr.txt를 읽기 모드로 open
	if((fp = fopen("ssu_expr.txt", "r")) == NULL) {
		fprintf(stderr, "fopen error for ssu_expr.txt\n");
		exit(1);
	}

	//파일이 끝날 때까지
	while(!feof(fp)) {
		//fp가 가리키는 파일 읽기, 정수인지 체크 
		while((character = fgetc(fp)) != EOF && isdigit(character)) 
			//ascii code '0' == 48이므로 48을 빼준다
			number = 10 * number + character - 48;

		//표준출력으로 출력
		fprintf(stdout, " %d\n", number);
		number = 0;

		if(character != EOF) {
			//character를 문자 스트림에 다시 집어넣고
			ungetc(character, fp);
			//해당 문자를 다시 읽어옴
			operator = fgetc(fp);
			printf("Operator => %c\n", operator);
		}
	}

	fclose(fp);
	exit(0);
}
