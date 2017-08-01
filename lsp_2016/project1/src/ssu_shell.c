#include "ssu_shell.h"

struct ssu_function cmdList[] = {
	{"ssu_history", ssu_history},
	{"ssu_grep", ssu_grep} 
};

int h_option;
int g_option;
int NUMBER;
char *STRING;
char *FILE_NAME;
char *NAME;

int main(void) {
	char cmd[CMD_MAX];
	char cmd_temp[CMD_MAX];
   	char *cmd_tok[TOK_MAX];
	int cmd_cnt;
	struct timeval start_time;
	struct timeval end_time;
   	struct timeval diff_time;
	double exec_time;

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

		diff_time.tv_sec = end_time.tv_sec - start_time.tv_sec;
		diff_time.tv_usec = end_time.tv_usec - start_time.tv_usec;
		if(diff_time.tv_usec < 0) {
			diff_time.tv_sec -= 1;
			diff_time.tv_usec += 1000000;
		}		
		
		exec_time = (double) diff_time.tv_sec + (double) diff_time.tv_usec / 1000000.0;	
		printf("\ntime: %.3lf\n", exec_time);
	}

	exit(EXIT_SUCCESS);
}

int parsing(char *argv[], char cmd[]) {
	char *str;
   	char *cmd_tok[TOK_MAX];
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
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	int cmd_cnt = 0;
	int i;
	char *cmd_record[TOTAL_CMD]; 

	if((fp = fopen(HISTORY_FILE, "r")) != NULL) {
		while((nread = getline(&line, &len, fp)) != -1) {
			cmd_record[cmd_cnt] = (char *)malloc(sizeof(char) * nread);
			strcpy(cmd_record[cmd_cnt], line);
			cmd_record[cmd_cnt][nread - 1] = '\0';
			cmd_cnt++;	
		}
	
		fclose(fp);
		fp = NULL;
	}
	
	if(cmd_cnt == TOTAL_CMD) {
		for(i = 0; i < cmd_cnt - 1; i++)
			strcpy(cmd_record[i], cmd_record[i + 1]);
		cmd_cnt--;
	}
	
	cmd_record[cmd_cnt] = (char *)malloc(sizeof(char) * (strlen(cmd) + 1));
	strcpy(cmd_record[cmd_cnt], cmd);
	cmd_cnt++;	
	fp = fopen(HISTORY_FILE, "w");

	for(i = 0; i < cmd_cnt; i++) {
		fwrite(cmd_record[i], strlen(cmd_record[i]), sizeof(char), fp);
		fwrite("\n", strlen("\n"), sizeof(char), fp);
	}	

	fclose(fp);
	fp = NULL;

	for(i = 0; i < cmd_cnt; i++) {
		free(cmd_record[i]);
		cmd_record[i] = NULL;
	}
}

void ssu_history(int argc, char *argv[]) {
	int i;
	int j;	
	char tmp[CMD_MAX];

	h_option = 0;
	NUMBER = 0;
	for(i = 1; i < argc; i++) {
		if(argv[i][0] != '-') {
			print_usage(argv[0], argv[i]);
			exit(EXIT_FAILURE);
		}

		for(j = 1; j < (int) strlen(argv[i]); i++) {
			if(argv[i][j] == 'c')
				h_option |= H_OPTION_C;
			else if(argv[i][j] == 'n') {
				
				if(strcmp(argv[i], "-n")) {
					print_usage(argv[0], argv[i]);
					exit(EXIT_FAILURE);
				}

				if(i + 1 >= argc) {
					strcpy(tmp, argv[i]);
					strcat(tmp, " ");
					strcat(tmp, "[EMPTY]");
					print_usage(argv[0], tmp);
					exit(EXIT_FAILURE);
				}
				
				if(!is_number(argv[i + 1])) {
					strcpy(tmp, argv[i]);
					strcat(tmp, " ");
					strcat(tmp, argv[i + 1]);
					print_usage(argv[0], tmp); 
					exit(EXIT_FAILURE);
				}

				h_option |= H_OPTION_N;
				NUMBER = atoi(argv[i + 1]);
				i++;
			}
			else if(argv[i][j] == 'r')
				h_option |= H_OPTION_R;
			else {
				print_usage(argv[0], argv[i]);	
				exit(EXIT_FAILURE);
			}
		}
	}

	exec_history();
}

void exec_history(void) {
	FILE *fp = NULL;
	char *line = NULL;
	size_t len = 0;
	int cmd_cnt = 0;
	int i;
	char cmd_record[TOTAL_CMD][CMD_MAX];
	
	fp = fopen(HISTORY_FILE, "r");
	while(getline(&line, &len, fp) != -1) {
		strcpy(cmd_record[cmd_cnt], line);
		cmd_record[cmd_cnt][strlen(cmd_record[cmd_cnt]) - 1] = '\0';
		cmd_cnt++;	
	}
	
	if(h_option & H_OPTION_C)
		exec_history_c(cmd_record);

	if(h_option & H_OPTION_N)
		exec_history_n(cmd_record);


	if(h_option & H_OPTION_R) 
		exec_history_r(cmd_record);

	for(i = 0; i < cmd_cnt; i++) {
		fwrite(cmd_record[i], strlen(cmd_record[i]), sizeof(char), stdout);
		fwrite("\n", strlen("\n"), sizeof(char), stdout);
	}	

	fclose(fp);
}

void exec_history_c(char cmd_record[][CMD_MAX]) {
	struct ssu_history_c record_count[TOTAL_SIZE];

}

void exec_history_n(char cmd_record[][CMD_MAX]) {

}

void exec_history_r(char cmd_record[][CMD_MAX]) {

}

void ssu_grep(int argc, char *argv[]) {
	int i;
	int j;	
		
	g_option = 0;
	STRING = NULL;
	FILE_NAME = NULL;
	NAME = NULL;

	if(argc != 3) {
		fprintf(stderr, "Usage: %s STRING FILE... [OPTION]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	STRING = argv[1];
	FILE_NAME = argv[2];

	for(i = 3; i < argc; i++) {
		if(argv[i][0] != '-') {
			print_usage(argv[0], argv[i]);
			exit(EXIT_FAILURE);
		}

		for(j = 1; j < (int) strlen(argv[i]); i++) {
			if(argv[i][j] == 'd')
				g_option |= G_OPTION_D;
			else if(argv[i][j] == 'e') {
				
				if(strcmp(argv[i], "-e")) {
					print_usage(argv[0], argv[i]);
					exit(EXIT_FAILURE);
				}

				if(i + 1 >= argc) {
					print_usage(argv[0], "-e [EMPTY]");
					exit(EXIT_FAILURE);
				}

				g_option |= G_OPTION_E;
				NAME = argv[i + 1];
				i++;
			}
			else if(argv[i][j] == 'i')
				g_option |= G_OPTION_I;
			else if(argv[i][j] == 'r')
				g_option |= G_OPTION_R;
			else {
				print_usage(argv[0], argv[i]);	
				exit(EXIT_FAILURE);
			}
		}
	}
}

void print_usage(char *cmd, char *opt) {
	printf("unknown option %s\n", opt);

	if(strcmp(cmd, cmdList[0].cmd) == 0) {
		printf("Usage: %s [OPTION]\n", cmd);
		printf("  -c : print number of each history\n");
		printf("  -n NUMBER : print history maximum NUMBER\n");
		printf("  -r : print history in reverse order\n");
	}
	else if(strcmp(cmd, cmdList[1].cmd) == 0) {
		printf("Usage: %s STRING FILE... [OPTION]\n", cmd);
		printf("  -d : delete file that has STRING\n");
		printf("  -e NAME : don't check file that includes NAME in file name\n");
		printf("  -i : ignore case distinctions\n");
		printf("  -n : print line number with output lines\n");
		printf("  -r : check all files in directory\n");
	}
}

bool is_number(char *str) {
	int i;
	for(i = 0; i < (int) strlen(str); i++) {
		if(!isdigit(str[i]))
			return false;
	}

	return true;
}
