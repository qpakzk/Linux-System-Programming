#include "ssu_vim.h"

//-r, -w, -rw 옵션의 입력 여부를 판별하는 플래그, 비트연산
int rw_flag = 0;
//-t, -s, -d 옵션의 입력 여부를 판별하는 플래그, 비트연산
int vim_flag = 0; 
//SIGUSR1 시그널 수신 여부를 판별하는 플래그
bool SIGUSR1_flag = false;
//접근을 요청하고자 하는 파일 이름을 저장
char *fname = NULL;

int main(int argc, char *argv[]) {
	//SIGUSR1 시그널 등록	
	set_signal(SIGUSR1, SIGUSR1_handler);
	//옵션 처리
	check_option_vim(argc, argv);
	//-r, -w, -rw 옵션 별 실행
	do_ssu_vim();

	exit(EXIT_SUCCESS);
}

//ssu_vim의 usage와 맞는지 검사하고 입력받은 옵션을 분류하는 작업을 수행하는 함수
void check_option_vim(int argc, char *argv[]) {
	int i;
	char *opt;

	if(argc < 3) {
		fprintf(stderr, "USAGE: ssu_vim <FILENAME> <-r | -w | -rw> [OPTION]\n");
		exit(EXIT_FAILURE);
	}
	
	//접근을 요청하고자하는 파일저장
	fname = argv[1];

	//-r, -w, -rw 옵션 중 입력받은 옵션을 플래그에 저장
	if(!strcmp(argv[2], "-r"))
		rw_flag = R_FLAG;
	else if(!strcmp(argv[2], "-w"))
		rw_flag = W_FLAG;
	else if(!strcmp(argv[2], "-rw"))
		rw_flag = RW_FLAG;
	else {
		fprintf(stderr, "ERROR: Invalid option\n");
		exit(EXIT_FAILURE);
	}

	//-t, -s, -d 옵션 중 입력받은 옵션을 플래그에 저장
	for(i = 3; i < argc; i++) {
		opt = argv[i];
		if(!strcmp(opt, "-t"))
			vim_flag |= VIM_T_FLAG;
		else if(!strcmp(opt, "-s"))
			vim_flag |= VIM_S_FLAG;
		else if(!strcmp(opt, "-d"))
			vim_flag |= VIM_D_FLAG;
		else {
			fprintf(stderr, "ERROR: Invalid option %s\n", opt);
			exit(EXIT_FAILURE);
		}
	}
}

//-r, -w, -rw 옵션에 대한 실행을 관리하는 함수
void do_ssu_vim() {
	if(rw_flag == R_FLAG)
		read_vim();
	else if(rw_flag == W_FLAG)
		write_vim();
	else if(rw_flag == RW_FLAG)
		read_write_vim();
	else {
		fprintf(stderr, "ERROR: Invalid option\n");	
		fprintf(stderr, "USAGE: ssu_vim <FILENAME> <-r | -w | -rw> [OPTION]\n");
		exit(EXIT_FAILURE);
	}
}

//공유 파일에 대한 read를 수행하는 함수
void read_vim() {
	FILE *fp;
	char buf[BUF_SIZE];
	int fsize;

	fp = fopen(fname, "r");
	
	//-t 옵션을 입력받은 경우
	if(vim_flag & VIM_T_FLAG)
		execute_t_option();
	
	//파일 데이터를 표준 출력으로 출력
	fsize = fread(buf, sizeof(char), BUF_SIZE - 1, fp);
	buf[fsize] = 0;
	fwrite(buf, sizeof(char), strlen(buf), stdout);
}

//공유 파일에 대한 read를 수행하는 함수
void write_vim() {
	pid_t pid;
	struct stat statbuf;
	char *tmp_file = "tmp.c";
	int before_size, after_size;
	
	//디몬 프로세스가 실행하고 있지 않은 경우 예외처리
	if((pid = find_daemon()) == 0) {
		printf("where is ssu_ofm?\n");
		printf("ssu_vim error\n");
		exit(EXIT_FAILURE);
	}
	
	lstat(fname, &statbuf);
	//-t 옵션을 입력받은 경우
	if((vim_flag & VIM_T_FLAG) && (rw_flag != RW_FLAG))
		execute_t_option();	

	//-s 옵션을 입력받은 경우		
	if(vim_flag & VIM_S_FLAG)
		before_size = get_file_size(fname);

	//-d 옵션을 입력받은 경우
	if(vim_flag & VIM_D_FLAG)
		copy_file(tmp_file, fname);
			
	execute_vim(pid);

	//-t 옵션을 입력받은 경우
	if(vim_flag & VIM_T_FLAG)
		check_modificaiton(statbuf.st_mtime);	

	//-s 옵션을 입력받은 경우	
	if(vim_flag & VIM_S_FLAG) {
		after_size = get_file_size(fname);
		if(is_modified(statbuf.st_mtime))
			execute_s_option(before_size, after_size);
	}

	//-d 옵션을 입력받은 경우	
	if(vim_flag & VIM_D_FLAG) {
		if(is_modified(statbuf.st_mtime))
			execute_d_option(tmp_file);
		remove(tmp_file);
	}
}

//공유 파일에 대한 read, write을 수행하는 함수
void read_write_vim() {
	char y_or_n[32];
	read_vim();//read 수행
	
	while(1) {//yes인 경우에만 write 수행
		printf("Would you like to modify '%s'? (yes/no) : ", fname);
		fgets(y_or_n, 31, stdin);
		y_or_n[strlen(y_or_n) - 1] = 0;
		if(!strcmp(y_or_n, "yes")) {
			write_vim();
			break;
		}
		else if(!strcmp(y_or_n, "no"))
			break;
		else
			printf("try again...\n");
	}
}

//공유 파일에 대해 write을 요청하는 함수
void execute_vim(pid_t daemon_pid) {
	struct tm *cur_timeinfo;
	//디몬 프로세스에 SIGUSR1 시그널을 전송한다.
	kill(daemon_pid, SIGUSR1);
	//FIFO 파일에 요청하고자 하는 파일이름을 write한다.
	send_message();	
		
	//ssu_ofm으로부터 SIGUSR1 시그널을 수신할 때까지 대기상태에 빠진다.
	while(!SIGUSR1_flag) {
		sleep(1);
		printf("Waiting for Token...%s", fname);
		
		//-t 옵션을 입력받은 경우
		if(vim_flag & VIM_T_FLAG) {
			cur_timeinfo = get_current_time();
			printf("[%.24s]\n", asctime(cur_timeinfo));
		}
		else
			printf("\n");
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

//디몬 프로세스가 실행 중인지 찾는 함수
int find_daemon(void) {
	DIR* dirp;
	struct dirent* ent;
	pid_t pid;
	char path[BUF_SIZE], buf[BUF_SIZE];
	FILE *fp;
	int dsize;
	
	// /proc 디렉터리 open
	if((dirp = opendir("/proc")) == NULL) {
		fprintf(stderr, "opendir error\n");
		exit(EXIT_FAILURE);
	}

	while((ent = readdir(dirp)) != NULL) {
		//프로세스id가 숫자가 아닌 경우 예외처리
		if(!is_number(ent->d_name)) 
			continue;

		memset(path, 0, sizeof(buf));
		snprintf(path, BUF_SIZE, "/proc/%s/cmdline", ent->d_name);
		if((fp = fopen(path, "r")) != NULL) {
			memset(buf, 0, sizeof(buf));
			dsize = fread(buf, sizeof(char), BUF_SIZE - 1, fp);
			buf[dsize] = 0;
			// /proc/pid/cmdline에 ./ssu_ofm이 저장되어 있는 경우 디몬 프로세스가 실행 중이라는 의미
			if(!strcmp(buf, "./ssu_ofm")) {
				fclose(fp);
				closedir(dirp);
				pid = atoi(ent->d_name);
				return pid;
			}
			fclose(fp);
		}
	}

	closedir(dirp);
	return 0;
}

//-t 옵션에서 파일의 마지막 수정시간과 현재 시간을 출력해주는 함수
void execute_t_option(void) {
	struct stat statbuf;
	struct tm *cur_timeinfo, *mod_timeinfo;

	lstat(fname, &statbuf);
	
	mod_timeinfo = localtime(&statbuf.st_mtime);
	
	printf("##[Modification Time]##\n");
	printf("Last Modification time of \'%s\': [%.24s]\n", fname, asctime(mod_timeinfo));

	cur_timeinfo = get_current_time();
	printf("Current time: [%.24s]\n", asctime(cur_timeinfo));
	sleep(1);
}

//현재 시간을 찾는 함수
struct tm *get_current_time(void) {
	time_t cur_time;
	time(&cur_time);
	return localtime(&cur_time);
}

//-t 옵션에서 파일이 수정되었는지 여부를 출력해주는 함수
void check_modificaiton(time_t before_st_mtime) {
	printf("##[Modification Time]##\n");
	if(!is_modified(before_st_mtime))
		printf("- There was no modificaition.\n");
	else
		printf("- There was modificaition.\n");
}

//파일 접근 전 후의 수정 시간을 비교하여 파일이 수정되었지 여부를 알려주는 함수
bool is_modified(time_t before_st_mtime) {
	struct stat statbuf;
	lstat(fname, &statbuf);

	return before_st_mtime != statbuf.st_mtime;
}

//ssu_ofm으로부터 SIGUSR1 시그널을 전송받았을 때 실행되는 시그널 핸들러 함수
void SIGUSR1_handler(int signo, siginfo_t *info, void *context) {
	pid_t pid;
	char *argv[] = {"vim", fname, NULL};
	
	SIGUSR1_flag = true;
	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(EXIT_FAILURE);
	}
	else if(pid > 0) {
		wait(NULL);//자식 프로세스가 종료될 때까지 대기
		kill(info->si_pid, SIGUSR2);//vim 프로세스 종료 후 ssu_ofm에 SIGUSR2 전송
	}
	else {//자식 프로세스에서 exec 함수를 호출하여 vim 프로세스 실행
		execvp("vim", argv);
		exit(EXIT_SUCCESS);
	}
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

//FIFO 파일에 write하는 함수
void send_message(void) {
	int fd;
	char buf[BUF_SIZE];
	if((fd = open(FIFO_FILE, O_WRONLY)) < 0) {
		fprintf(stderr, "open error\n");
		exit(EXIT_FAILURE);
	}

	strcpy(buf, fname);
	write(fd, buf, strlen(buf));
	close(fd);
}

//파일 크기를 구하는 함수
int get_file_size(char *file) {
	FILE *fp = fopen(file, "r");
	int size;

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fclose(fp);
	return size;
}

//-s 옵션에서 파일 접근 전, 후의 파일 크기를 출력하는 함수
void execute_s_option(int before_size, int after_size) {
	printf("##[File Size]##\n");
	printf("-- Before modification : %d(bytes)\n", before_size);
	printf("-- after modification : %d(bytes)\n", after_size);
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

//old_file 파일 데이터를 new_file 파일 데이터로 복사하는 함수
void execute_d_option(char *before_file) {
	pid_t pid;
	char *argv[] = {"diff", before_file, fname, NULL};


	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(EXIT_FAILURE);
	}
	else if(pid > 0) 
		wait(NULL);//자식 프로세스가 종료할 때까지 대기
	else {//자식 프로세스에서  exec을 호출해서 diff 명령어를 수행
		printf("##[Compare with Previous File]##\n");
		execvp("diff", argv);
		exit(EXIT_SUCCESS);
	}
}
