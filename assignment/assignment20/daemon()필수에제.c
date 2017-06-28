#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

int ssu_daemon_init(void);

int main(void) {
	pid_t pid;
	
    //process id
	pid = getpid();
	printf("parent process : %d\n", pid);
	printf("daemon process initialization\n");	

    //daemon 생성
	if(ssu_daemon_init() < 0) {
		fprintf(stderr, "ssu_daemon_init failed\n");
		exit(1);
	}

	exit(0);
}

int ssu_daemon_init(void) {
	pid_t pid;
	int fd, maxfd;
    
    //자식 프로세스 생성
	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}	
	else if(pid != 0)
		exit(0);

    //process id
	pid = getpid();
	printf("process %d running as daemon\n", pid);
    //session 생성
	setsid();
    //SIGTTIN, SIGTTOU, SIGTSTP 시그널 무시
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
    //파일 디스크립터 테이블 크기
	maxfd = getdtablesize();
    
    //모든 fd 닫음
	for(fd = 0; fd < maxfd; fd++)
		close(fd);

    //masking 안함
	umask(0);
    //작업 디렉터리를 루트로 설정
	chdir("/");
    //표준입력, 표준출력, 표준에러를 /dev/null로 redirection
	fd = open("/dev/null", O_RDWR);
	dup(0);
	dup(0);
	return 0;
}
