#include "ssu_shell.h"

int main(void) {
	char cmd[N];
	char cmd_temp[N];
	char *cmd_token[100] = {NULL};
	int count, count_temp, i, j;
	int incr;
	struct timeval start_point, end_point;
	double exec_time;
	char temp[100][1000] = {0};

	while(1) {
        //프롬프트 출력
		printf("20142577 $ ");
        //cmd에 명령 입력
		fgets(cmd, sizeof(cmd), stdin);
        //엔터 입력 시 프롬프트 유지
		if(strcmp(cmd, "\n") == 0)
			continue;
		cmd[strlen(cmd) - 1] = '\0';	
		strcpy(cmd_temp, cmd);
        //명령을 파싱하고 명령어 이름과 인자들 개수 반환
		count = parsing(cmd_temp, cmd_token);		
			
		for(i = 0; i < count; i++)
			strcpy(temp[i], cmd_token[i]);
		
		if(strcmp(cmd_token[0], "ssu_sed") == 0) {//ssu_sed 명령어를 입력받았을 경우
			incr = 1;
			count_temp = count;
            //문자열 끝에 \\(역슬래시)가 있을 경우
            //띄어쓰기가 있는 디렉토리명, 파일명으로 처리
			for(i = 1; i < count; i++) {
				while(cmd_token[i][(int) strlen(cmd_token[i]) - 1] == '\\') {
					if(i + 1 < count && incr + 1 < count) {
                        //역슬래시를 띄어쓰기로 만들고
                        //두 문자열을 하나로 합침
						temp[incr][strlen(temp[incr]) - 1] = ' ';	
						strcat(temp[incr], temp[incr + 1]);
					}
					else {
						temp[incr][(int) strlen(temp[incr]) - 1] = '\0';	
						break;
					}

					for(j = incr + 1; j < count_temp - 1; j++)
						strcpy(temp[j], temp[j + 1]);
					i++;
					count_temp--;
				}
				incr++;
			}
            
            //띄어쓰기가 포함된 디렉터리/파일명 저장 완료
			for(i = 1; i < incr; i++) 
				cmd_token[i] = temp[i];
			count = incr;
		}
	
        //명령이름과 인자 포함 10개가 넘으면 오류 출력
		if(count > 10) {
			printf("ERROR: can input maximum 10 including program's name and parameters.\n");
			continue;
		}

        //동작 시간 측정
		gettimeofday(&start_point, NULL);	
		if(strcmp(cmd_token[0], "ssu_sed") == 0)
			ssu_sed(count, cmd_token);//ssu_sed 명령 수행
		else
			system(cmd);//ssu_sed 명령 이외의 명령 수행
		gettimeofday(&end_point, NULL);
        
        //동작시간 계산 && 출력
		exec_time = (double)(end_point.tv_sec) + (double)(end_point.tv_usec) / 1000000.0 - ((double)(start_point.tv_sec) + (double)(start_point.tv_usec) / 1000000.0);	
		printf("\ntime: %.3lf\n", exec_time);	
	}	

	exit(0);
}

//명령을 파싱하는 함수
int parsing(char *cmd, char *cmd_token[]) {
	int count = 0;
	cmd_token[count]  = strtok(cmd, " \t");
	while(cmd_token[count] != NULL) {
		count++;
		cmd_token[count] = strtok(NULL, " \t");
	}	

	return count;
}

