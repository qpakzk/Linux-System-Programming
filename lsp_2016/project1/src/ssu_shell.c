#include "ssu_shell.h"

struct ssu_function cmdList[] = {
	{"ssu_history", ssu_history},
	{"ssu_grep", ssu_grep} };

int main(void) {
	char cmd[CMD_MAX], cmd_temp[CMD_MAX], *cmd_tok[TOK_MAX];
	int cmd_cnt;
	struct timeval start_time, end_time, exec_time;
	double diff_time;

	while(1) {
		printf("20142577 $ ");
		fgets(cmd, CMD_MAX, stdin);
		if(cmd[0] == '\n')
			continue;
		cmd[strlen(cmd) - 1] = '\0';
		strcpy(cmd_temp, cmd);	
		cmd_cnt = parsing(cmd_tok, cmd_temp);
		
		if(cmd_cnt == -1) {
			fprintf(stderr, "ERROR: There are too many arguments. Please input less than or equal to 10 arguments.\n");
			continue;
		}		

		add_history(cmd);	
		
		gettimeofday(&start_time, NULL);
		if(strcmp(cmd_tok[0], cmdList[0].cmd) == 0)
			cmdList[0].func(cmd_cnt, cmd_tok);
		else if(strcmp(cmd_tok[0], cmdList[1].cmd) == 0)
			cmdList[1].func(cmd_cnt, cmd_tok);
		else
			system(cmd);
		gettimeofday(&end_time, NULL);

		exec_time.tv_sec = end_time.tv_sec - start_time.tv_sec;
		exec_time.tv_usec = end_time.tv_usec - start_time.tv_usec;
		if(exec_time.tv_usec < 0) {
			exec_time.tv_sec -= 1;
			exec_time.tv_usec += 1000000;
		}		
		
		diff_time = (double) exec_time.tv_sec + (double) exec_time.tv_usec / 1000000.0;	
		printf("\ntime: %.3lf\n", diff_time);
	}

	exit(0);
}

int parsing(char *argv[], char cmd[]) {
	char *str, *cmd_tok[TOK_MAX];
	int i;

	int tok_count = 0;	
	str = strtok(cmd, " ");
	while(str != NULL) {
		cmd_tok[tok_count] = str;
		tok_count++;
		if(tok_count > 10)
			return -1;
		str = strtok(NULL, " ");
	}
	
	for(i = 0; i < tok_count; i++) 
		argv[i] = cmd_tok[i];

	return tok_count;
}

void add_history(char cmd[]) {
	FILE *fp = NULL;
	int  len = 0, cnt = 0, i;
	char *file_data = NULL, *user_cmd[TOTAL_CMD], temp[CMD_MAX + 1];

	if((fp = fopen(HISTORY_FILE, "r")) != NULL) {
		fseek(fp, 0, SEEK_END);	
		len = ftell(fp);
		file_data = (char *)malloc(sizeof(char) * (len + 1));	

		fseek(fp, 0, SEEK_SET);	
		fread(file_data, sizeof(char), len, fp);
		file_data[len] = '\0';
		if(len > 0) {
			cnt = count_cmd(file_data);
			make_cmd_list(file_data, user_cmd, &cnt);
		}

		fclose(fp);

		free(file_data);
		file_data = NULL;
	}

	user_cmd[cnt] = cmd;
	cnt++;

	fp = fopen(HISTORY_FILE, "w");
	fseek(fp, 0, SEEK_SET);
	for(i = 0; i < cnt; i++) {
		strcpy(temp, user_cmd[i]);
		strcat(temp, "\n");	
		fwrite(temp, sizeof(char), strlen(temp), fp);
	}
	fclose(fp);
}

int count_cmd(char data[]) {
	int i = 0, cnt = 0;
	while(data[i] != '\0') {
		if(data[i] == '\n')
			cnt++;
		i++;
	}
	
	return cnt;
}

void make_cmd_list(char data[], char *cmd_list[], int *cnt) {
	int idx = 0, i, j;
	char *str = NULL;

	if(*cnt == 100) {
		while(data[idx] != '\n')
			idx++;
		idx++;			
		*cnt = 99;
	}	

	for(i = 0; i < *cnt; i++) {
		str = (char *)malloc(CMD_MAX);
		j = 0;

		while(data[idx] != '\n') {
			str[j] = data[idx];
			idx++;
			j++;
		}
		idx++;
		str[j] = '\0';

		cmd_list[i] = str;
		free(str);
		str = NULL;	
	}
}


void ssu_history(int argc, char *argv[]) {
		
}

void ssu_grep(int argc, char *argv[]) {

}
