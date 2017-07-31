#include "ssu_ofm.h"

//-l, -t, -n, -p, -id 옵션의 입력 여부를 판별하는 플래그, 비트연산
int ofm_flag = 0;
//공유파일
char *fname = NULL;
//-n 옵션의 인자 = queue의 최대 크기
char *NUMBER = NULL;
//-p 옵션의 인자 = 디렉터리 이름
char *DIRECTORY = NULL;
//프로세스id queue
Queue pid_queue;

int main(int argc, char *argv[]) {
	//queue 초기화
	init_queue(&pid_queue);
	//queue 크기를 디폴트로 16 저장	
	pid_queue.max_size = 16;
	//옵션 처리
	check_option_ofm(argc, argv);
	
	//FIFO 파일 생성
	make_fifo();
	//SIGUSR1, SIGUSR2 시그널 등록
	set_signal(SIGUSR1, SIGUSR1_handler);
	set_signal(SIGUSR2, SIGUSR2_handler);

	printf("Daemon Process Initialization.\n");
	//디몬 프로세스 생성	
	if(ssu_daemon_init() < 0) { 
		fprintf(stderr, "ssu_daemon_init failed\n");
		exit(EXIT_FAILURE);
	}
	//디몬 프로세스 생성 후 log 파일에 써야할 내용 기록
	write_log_init();	

	//디몬 프로세스가 SIGKILL 시그널을 받을 때까지 실행
	while(1);

	exit(EXIT_SUCCESS);
}

//ssu_ofm의 usage와 맞는지 검사하고 입력받은 옵션을 분류하는 작업을 수행하는 함수
void check_option_ofm(int argc, char *argv[]) {
	int i;
	char *opt;

	if(argc < 2) {
		fprintf(stderr, "USAGE: ssu_ofm <FILENAME> [OPTION]\n");
		exit(EXIT_FAILURE);
	}
	
	//공유 파일 이름 저장
	fname = argv[1];
	
	//-l, -t, -n, -p, -id 옵션 중 입력받은 옵션을 플래그에 저장
	for(i = 2; i < argc; i++) {
		opt = argv[i];
		if(!strcmp(opt, "-l"))
			ofm_flag |= OFM_L_FLAG;
		else if(!strcmp(opt, "-t"))
			ofm_flag |= OFM_T_FLAG;
		else if(!strcmp(opt, "-n")) {
			if(i + 1 < argc && is_number(argv[i + 1])) {
				ofm_flag |= OFM_N_FLAG;
				NUMBER = argv[i + 1];//queue 크기 업데이트
				pid_queue.max_size = atoi(NUMBER);
				i++;
			}
			else {
				fprintf(stderr, "Invalid option\n");
				fprintf(stderr, "-n USAGE: -n <NUMBER>\n");
				exit(EXIT_FAILURE);
			}
		}
		else if(!strcmp(opt, "-p")) {
			ofm_flag |= OFM_P_FLAG;
			if(i + 1 < argc && is_dir(argv[i + 1])) {
				DIRECTORY = argv[i + 1];//디렉터리 이름 저장
				i++;
			}
			else {
				fprintf(stderr, "Invalid option\n");
				fprintf(stderr, "-p USAGE: -p <DIRECTORY>\n");
				exit(EXIT_FAILURE);
			}
		}
		else if(!strcmp(opt, "-id"))
			ofm_flag |= OFM_ID_FLAG;
		else {
			fprintf(stderr, "ERROR: Invalid option %s\n", opt);
			exit(EXIT_FAILURE);
		}
	}
}

//매개변수로 입력받은 문자열이 숫자인지 판별하는 함수
bool is_number(char *str) {
	int i;
	for(i = 0; i < strlen(str); i++) {
		if(!isdigit(str[i]))
			return false;
	}

	return true;
}

//디렉터리 이름인지 판별하는 함수
bool is_dir(char *str) {
	if(*str == '-')
		return false;
	return true;
}

//log 파일의 경로를 설정해주는 함수
void set_log_path(char *path, char *log, char *dir) {
	struct stat statbuf;
	
	//-p 옵션을 입력받은 경우		
	if(ofm_flag & OFM_P_FLAG) {
		lstat(dir, &statbuf);
		//dir 이름의 디렉터리가 존재하지 않는 경우
		if(access(dir, F_OK) || !S_ISDIR(statbuf.st_mode))
			mkdir(dir, 0777);//디렉터리 생성
		snprintf(path, BUF_SIZE, "%s/%s", dir, log);
	}
	else
		snprintf(path, BUF_SIZE, "%s", log);
}

//디몬 프로세스를 생성하는 함수
int ssu_daemon_init(void) {
	pid_t pid;
	int fd, maxfd;

	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if(pid != 0)//백그라운드에서 수행되어야 한다.
		exit(0);
	
	pid = getpid();
	setsid();//프로세스 그룹에서 탈퇴하고 새로운 프로세스 그룹을 생성한다.
	//터미널 입출력 시그널을 무시한다.
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	maxfd = getdtablesize();
	
	//열려있는 모든 파일 디스크립터를 닫는다.
	for(fd = 0; fd < maxfd; fd++)
		close(fd);
	
	//파일 모드 생성 마스크를 해제한다.
	umask(0);
	//현재 디렉터리를 루트 디렉터리로 설정한다.
	/* 본 설계과제에서는 사용자 레벨에서 디몬 프로세스가 수행되어야 하므로 
	 * 루트 디렉터리로 설정하는 부분은 생략한다.
	 */
	//chdir("/");
	
	//표준 입출력과 오류출력을 /dev/null로 리다이렉션한다.
	fd = open("/dev/null", O_RDWR);
	dup(0);
	dup(0);

	
	return 0;
}

//디몬 프로세스가 생성되자마자 써야 할 내용을 log 파일에 write하는 함수
void write_log_init(void) {
	FILE *fp;
	pid_t pid;
	char path[BUF_SIZE], buf[BUF_SIZE], queue_size_buf[BUF_SIZE];

	set_log_path(path, LOG_FILE, DIRECTORY);
	if((fp = fopen(LOG_FILE, "w")) == NULL)
		exit(EXIT_FAILURE);

	pid = getpid();
	memset(buf, 0, sizeof(buf));
	if(ofm_flag & OFM_T_FLAG)//-t 옵션을 입력받아 시간을 추가
		snprintf(buf, BUF_SIZE, "[%.24s] <<Daemon Process Initialization with pid : %d>>\n", get_current_time(), pid);
	else
		snprintf(buf, BUF_SIZE, "<<Daemon Process Initialization with pid : %d>>\n", pid);

	memset(queue_size_buf, 0, sizeof(buf));
	if(ofm_flag & OFM_N_FLAG)//-n 옵션을 입력받아 queue의 최대 크기 변경
		snprintf(queue_size_buf, BUF_SIZE, "Initialized Queue Size : %s\n", NUMBER);
	else
		snprintf(queue_size_buf, BUF_SIZE, "Initialized with Default Value : %d\n", 16);
	strcat(buf, queue_size_buf);	
	fwrite(buf, sizeof(char), strlen(buf), fp);

	fclose(fp);
	fp = NULL;
}

//현재 시간을 찾는 함수
char *get_current_time() {
	time_t cur_time;
	struct tm *cur_timeinfo;

	time(&cur_time);
	cur_timeinfo = localtime(&cur_time);

	return asctime(cur_timeinfo);
}

//sigaction의 인자로 들어갈 sigaction 구조체 변수를 초기화하고 sigaction 함수를 호출하는 함수
void set_signal(int signo, void (*handler)(int, siginfo_t *, void *)) {
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = handler;
	act.sa_flags = SA_SIGINFO;
	sigemptyset(&act.sa_mask);

	sigaction(signo, &act, NULL);
}

//ssu_vim으로부터 SIGUSR1 시그널을 전송받았을 때 실행되는 시그널 핸들러 함수
void SIGUSR1_handler(int signo, siginfo_t *info, void *context) {
	//SIGUSR1을 보낸 프로세스의 id
	pid_t pid = info->si_pid, next_pid;
	char buf[BUF_SIZE];

	memset(buf, 0, sizeof(buf));
	shared_file(buf);//ssu_vim에서 접근을 요청하는 파일
	//요청 상황을 log 파일에 기록 
	write_log_requested(pid, buf);
	
	//공유 파일과 요청한 파일이 다르면
	if(strcmp(fname, buf)) {
		kill(pid, SIGKILL);//ssu_vim에 SIGKILL 전송
		return;
	}

	if(!is_running()) {//공유 파일에 접근 중인 프로세스가 없는 경우
		if(!empty(&pid_queue)) {//queue가 비어 있지 않은 경우
			next_pid = dequeue(&pid_queue);//queue에서 다음으로 실행할 pid를 가져옴
			enqueue(&pid_queue, pid);//가장 최근에 요청받은 프로세스를 queue에 저장
			kill(next_pid, SIGUSR1);//ssu_vim에 SIGUSR1 전송
		}
		else
			kill(pid, SIGUSR1);//ssu_vim에 SIGUSR1 전송
	}
	else//공유 파일에 접근 중인 프로세스가 있는 경우
		enqueue(&pid_queue, pid);//가장 최근에 요청받은 프로세스를 queue에 저장
}

//FIFO 파일을 생성하는 함수
void make_fifo(void) {
	//FIFO 파일과 동일한 이름의 파일이 존재한다면 	
	if(access(FIFO_FILE, F_OK) == 0)
		remove(FIFO_FILE);//해당 파일 삭제
	//FIFO 파일 생성	
	if(mkfifo(FIFO_FILE, 0666) < 0) {
		fprintf(stderr, "mkfifo error\n");
		exit(EXIT_FAILURE);
	}
}

//sender로부터 FIFO에 입력된 데이터를 read하는 함수
void shared_file(char *buf) {
	int fd;

	//FIFO 파일 open
	if((fd = open(FIFO_FILE, O_RDWR)) < 0) {
		fprintf(stderr, "open error\n");
		exit(EXIT_FAILURE);
	}
	
	//buf에 파일의 데이터를 read
	read(fd, buf, BUF_SIZE);
	
	close(fd);
}

//ssu_vim으로부터 SIGUSR1 시그널을 전송받았을 때 log 파일에 써야할 데이터를 write하는 함수
void write_log_requested(pid_t pid, char *shared) {
	FILE *fp;
	char buf[BUF_SIZE], id_buf[BUF_SIZE], path[BUF_SIZE];
	set_log_path(path, LOG_FILE, DIRECTORY);
		
	memset(buf, 0, sizeof(buf));
	if(ofm_flag & OFM_T_FLAG)//-t 옵션을 입력받아 시간을 추가
		snprintf(buf, BUF_SIZE, "[%.24s] Requested Process ID : %d, Requested Filename : %s\n", get_current_time(), pid, shared);
	else
		snprintf(buf, BUF_SIZE, "Requested Process ID : %d, Requested Filename : %s\n", pid, shared);
	
	memset(id_buf, 0, sizeof(id_buf));
	if(ofm_flag & OFM_ID_FLAG) {//-id 옵션을 입력받아 id 데이터 추가
		execute_id_option(id_buf);				
		strcat(buf, id_buf);
	}
		
	if((fp = fopen(path, "a+")) == NULL) {
		fprintf(stderr, "fopen error\n");
		exit(EXIT_FAILURE);
	}
	
	//log 파일에 write
	fwrite(buf, sizeof(char), strlen(buf), fp);
	fclose(fp);
	
}

//-id 옵션에서 SIGUSR2를 전송한 프로세스의 유저이름, UID, GID를 log 파일에 write할 준비를 함수
void execute_id_option(char *buf) {
	uid_t uid = getuid();//유저 아이디
	struct passwd *pw_info;
	//uid를 이용하여 passwd 구조체 얻음
	if((pw_info = getpwuid(uid)) == NULL) {
		fprintf(stderr, "getpwnam error\n");
		exit(EXIT_FAILURE);
	}
	
	//유저 이름 uid, gid 값 저장
	snprintf(buf, BUF_SIZE, "User : %s, UID : %d, GID : %d\n", pw_info->pw_name, pw_info->pw_uid, pw_info->pw_gid);
}

//현재 공유 파일이 다른 프로세스에 의해 접근 중인지 확인하는 함수
bool is_running() {
	DIR* dirp;
	struct dirent* ent;
	char path[BUF_SIZE];
	FILE *fp;
	char argv0[BUF_SIZE], argv1[BUF_SIZE];
	int size;
	
	// /proc 디렉터리 open
	if((dirp = opendir("/proc")) == NULL) {
		fprintf(stderr, "openddir error for /proc\n");
		exit(EXIT_FAILURE);
	}
	
	while((ent = readdir(dirp)) != NULL) {
		if(!is_number(ent->d_name)) 
			continue;

		memset(path, 0, sizeof(path));
		snprintf(path, BUF_SIZE, "/proc/%s/cmdline", ent->d_name);
		if((fp = fopen(path, "r")) != NULL) {
			size = fread(argv0, sizeof(char), BUF_SIZE - 1, fp);
			argv0[size] = 0;
			if(size > 0) {//vim 프로세스가 실행 중인 경우
				if(!strcmp(argv0, "vim")) {
					//vim을 통해 open한 공유 파일도 같은 경우 => 실행
 					strcpy(argv1, argv0 + strlen(argv0) + 1);
					argv1[strlen(argv1)] = 0;
					if(!strcmp(argv1, fname)) {
						fclose(fp);
						closedir(dirp);
						return true;
					}
				}
			}
			fclose(fp);
		}
	}
	closedir(dirp);

	return false;
}

//ssu_vim으로부터 SIGUSR2 시그널을 전송받았을 때 실행되는 시그널 핸들러 함수
void SIGUSR2_handler(int signo, siginfo_t *info, void *context) {
	pid_t pid = info->si_pid, next_pid;
	//SIGUSR2 시그널을 받았을 때 log 파일에 써야할 데이터 기록
	write_log_finished(pid);

	//queue가 비어 있는 경우
	if(empty(&pid_queue))
		return;

	//다음에 실행할 프로세스를 queue에서 가져옴
	next_pid = dequeue(&pid_queue);
	//ssu_vim에 SIGUSR1 전송
	kill(next_pid, SIGUSR1);
}

//ssu_vim으로부터 SIGUSR2 시그널을 전송받았을 때 log 파일에 써야할 데이터를 write하는 함수
void write_log_finished(pid_t pid) {
	FILE *fp, *fp2;
	char *cur_time = NULL;
	char buf[BUF_SIZE], new_file[BUF_SIZE], tmp[BUF_SIZE], path[BUF_SIZE], data[BUF_SIZE];
	int len;
	set_log_path(path, LOG_FILE, DIRECTORY);
		
	cur_time = get_current_time();
	memset(buf, 0, sizeof(buf));
	if(ofm_flag & OFM_T_FLAG) //-t 옵션을 입력받아 시간을 추가
		snprintf(buf, BUF_SIZE, "[%.24s] Finished Process ID : %d\n", cur_time, pid);
	else
		snprintf(buf, BUF_SIZE, "Finished Process ID : %d\n", pid);
	
	if((fp = fopen(path, "a+")) == NULL) {
		fprintf(stderr, "fopen error\n");
		exit(EXIT_FAILURE);
	}
	
	fwrite(buf, sizeof(char), strlen(buf), fp);

	if(ofm_flag & OFM_L_FLAG) {//-l 옵션을 입력받아 시간을 파일이름으로 하는 log 파일 생성
		snprintf(tmp, BUF_SIZE, "[%.24s]", cur_time);	
		set_log_path(new_file, tmp, DIRECTORY);
		copy_file(new_file, path);
	}

	fclose(fp);
}

//old_file 파일 데이터를 new_file 파일 데이터로 복사하는 함수
void copy_file(char *new_file, char *old_file) {
	FILE *nfp = fopen(new_file, "w");
	FILE *ofp = fopen(old_file, "r");
	char buf[BUF_SIZE];

	memset(buf, 0, sizeof(buf));
	fread(buf, sizeof(char), BUF_SIZE, ofp);
	fwrite(buf, sizeof(char), strlen(buf), nfp);	
	
	fclose(ofp);
	fclose(nfp);
}
