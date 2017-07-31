#ifndef __SSU_VIM_H__
#define __SSU_VIM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>
#include <dirent.h>
#include <time.h>

//-r, -w, -rw 옵션 플래그 값
#define R_FLAG 0x1
#define W_FLAG 0x2
#define RW_FLAG 0x4

//-t, -s, -d 옵션 플래그 값
#define VIM_T_FLAG 0x1
#define VIM_S_FLAG 0x2
#define VIM_D_FLAG 0x4

#define BUF_SIZE 1024
#define TIME_SIZE 64

//FIFO 파일 이름
#define FIFO_FILE "ssu_fifo"

//ssu_vim의 usage와 맞는지 검사하고 입력받은 옵션을 분류하는 작업을 수행하는 함수
void check_option_vim(int argc, char *argv[]);
//-r, -w, -rw 옵션에 대한 실행을 관리하는 함수
void do_ssu_vim(void);
//공유 파일에 대한 read를 수행하는 함수
void read_vim(void);
//공유 파일에 대한 read를 수행하는 함수
void write_vim(void);
//공유 파일에 대한 read, write을 수행하는 함수
void read_write_vim(void);
//공유 파일에 대해 write을 요청하는 함수
void execute_vim(pid_t daemon_pid);
//매개변수로 입력받은 문자열이 숫자인지 판별하는 함수
bool is_number(char *str);
//디몬 프로세스가 실행 중인지 찾는 함수
int find_daemon(void);
//-t 옵션에서 파일의 마지막 수정시간과 현재 시간을 출력해주는 함수
void execute_t_option(void);
//현재 시간을 찾는 함수
struct tm *get_current_time(void);
//-t 옵션에서 파일이 수정되었는지 여부를 출력해주는 함수
void check_modificaiton(time_t before_st_mtime);
//파일 접근 전 후의 수정 시간을 비교하여 파일이 수정되었지 여부를 알려주는 함수
bool is_modified(time_t before_st_mtime);
//ssu_ofm으로부터 SIGUSR1 시그널을 전송받았을 때 실행되는 시그널 핸들러 함수
void SIGUSR1_handler(int signo, siginfo_t *info, void *context);
//sigaction의 인자로 들어갈 sigaction 구조체 변수를 초기화하고 sigaction 함수를 호출하는 함수
void set_signal(int signo, void (*handler)(int, siginfo_t *, void *));
//FIFO 파일에 write하는 함수
void send_message(void);
//파일 크기를 구하는 함수
int get_file_size(char *file);
//-s 옵션에서 파일 접근 전, 후의 파일 크기를 출력하는 함수
void execute_s_option(int before_size, int after_size);
//old_file 파일 데이터를 new_file 파일 데이터로 복사하는 함수
void copy_file(char *new_file, char *oldfile);
//old_file 파일 데이터를 new_file 파일 데이터로 복사하는 함수
void execute_d_option(char *before_file);

#endif
