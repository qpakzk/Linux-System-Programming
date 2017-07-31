#ifndef __SSU_SHELL_H__
#define __SSU_SHELL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CMD_MAX 1000000

struct ssu_function {
	char *cmd;
	void (*func)(int, char *[]);
};

int parsing(char *argv[], char cmd[]);
void ssu_history(int argc, char *argv[]);
void ssu_grep(int argc, char *argv[]);

#endif
