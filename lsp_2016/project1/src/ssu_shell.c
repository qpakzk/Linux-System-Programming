#include "ssu_shell.h"

struct ssu_function cmdList[] = {
	{"ssu_history", ssu_history},
	{"ssu_grep", ssu_grep}
};

int main(void) {
	char cmd[CMD_MAX], cmd_temp[CMD_MAX], *cmd_token[CMD_MAX];
	int cmd_cnt;

	while(1) {
		printf("20142577 $ ");
		fgets(cmd, CMD_MAX, stdin);
		if(cmd[0] == '\n')
			continue;
		cmd[strlen(cmd) - 1] = '\0';
		strcpy(cmd_temp, cmd);	
		cmd_cnt = parsing(cmd_token, cmd_temp);
		
	}

	exit(0);
}

int parsing(char *argv[], char cmd[]) {
	char *str;
	int tok_count = 0;	
	str = strtok(cmd, " ");
}

void ssu_history(int argc, char *argv[]) {

}

void ssu_grep(int argc, char *argv[]) {

}
