#ifndef __SSU_SHELL_H__
#define __SSU_SHELL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#define CMD_MAX 1000000
#define TOK_MAX 10
#define TOTAL_CMD 100
#define HISTORY_FILE "history_file.txt"

struct ssu_function {
	char *cmd;
	void (*func)(int, char *[]);
};

int parsing(char *argv[], char cmd[]);
void add_history(char cmd[]);
int count_cmd(char data[]);
void make_cmd_list(char data[], char *cmd_list[], int *cnt);
void ssu_history(int argc, char *argv[]);
void ssu_grep(int argc, char *argv[]);

#endif
