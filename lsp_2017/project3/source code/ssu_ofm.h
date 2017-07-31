#ifndef __SSU_OFM_H__
#define __SSU_OFM_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <dirent.h>
#include "queue.h"

//-l, -t, -n, -p, -id 옵션 플래그 값
#define OFM_L_FLAG 0x01
#define OFM_T_FLAG 0x02
#define OFM_N_FLAG 0x04
#define OFM_P_FLAG 0x08
#define OFM_ID_FLAG 0x10

#define BUF_SIZE 1024
//FIFO 파일 이름
#define FIFO_FILE "ssu_fifo"
//log 파일 이름
#define LOG_FILE "ssu_log.txt"

//ssu_ofm의 usage와 맞는지 검사하고 입력받은 옵션을 분류하는 작업을 수행하는 함수
void check_option_ofm(int argc, char *argv[]);
//매개변수로 입력받은 문자열이 숫자인지 판별하는 함수
bool is_number(char *str);
//디렉터리 이름인지 판별하는 함수
bool is_dir(char *str);
//log 파일의 경로를 설정해주는 함수
void set_log_path(char *path, char *log, char *dir);
//디몬 프로세스를 생성하는 함수
int ssu_daemon_init(void);
//디몬 프로세스가 생성되자마자 써야 할 내용을 log 파일에 write하는 함수
void write_log_init(void);
//현재 시간을 찾는 함수
char *get_current_time();
//sigaction의 인자로 들어갈 sigaction 구조체 변수를 초기화하고 sigaction 함수를 호출하는 함수
void set_signal(int signo, void (*handler)(int, siginfo_t *, void *));
//ssu_vim으로부터 SIGUSR1 시그널을 전송받았을 때 실행되는 시그널 핸들러 함수
void SIGUSR1_handler(int signo, siginfo_t *si, void *context);
//FIFO 파일을 생성하는 함수
void make_fifo(void);
//sender로부터 FIFO에 입력된 데이터를 read하는 함수
void shared_file(char *buf);
//ssu_vim으로부터 SIGUSR1 시그널을 전송받았을 때 log 파일에 써야할 데이터를 write하는 함수
void write_log_requested(pid_t pid, char *shared);
//-id 옵션에서 SIGUSR2를 전송한 프로세스의 유저이름, UID, GID를 log 파일에 write할 준비를 함수
void execute_id_option(char *buf);
//현재 공유 파일이 다른 프로세스에 의해 접근 중인지 확인하는 함수
bool is_running();
//ssu_vim으로부터 SIGUSR2 시그널을 전송받았을 때 실행되는 시그널 핸들러 함수
void SIGUSR2_handler(int signo, siginfo_t *info, void *context);
//ssu_vim으로부터 SIGUSR2 시그널을 전송받았을 때 log 파일에 써야할 데이터를 write하는 함수
void write_log_finished(pid_t pid);

void copy_file(char *new_file, char *old_file);

#endif

