#ifndef __SSU_SHELL_H__
#define __SSU_SHELL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <ctype.h>

#define CMD_MAX 1000000
#define TOK_MAX 10
#define TOTAL_CMD 100
#define HISTORY_FILE "history_file.txt"

#define H_OPTION_C 0x1
#define H_OPTION_N 0x2
#define H_OPTION_R 0x4

#define G_OPTION_D 0x01
#define G_OPTION_E 0x02
#define G_OPTION_I 0x04
#define G_OPTION_N 0x08
#define G_OPTION_R 0x10

struct ssu_function {
	char *cmd;
	void (*func)(int, char *[]);
};

struct ssu_history_c {
	char cmd[TOTAL_CMD][CMD_MAX];
	int count;
};
int parsing(char *argv[], char cmd[]);
void add_history(char cmd[]);
void ssu_history(int argc, char *argv[]);
void exec_history(void);
void exec_history_c(char cmd_record[][CMD_MAX]);
void exec_history_n(char cmd_record[][CMD_MAX]);
void exec_history_r(char cmd_record[][CMD_MAX]);
void ssu_grep(int argc, char *argv[]);
void print_usage(char *cmd, char *opt);
bool is_number(char *str);

#endif
