#include "ssu_sed.h"

//ssu_sed 프로그램을 실행시키는 함수
void ssu_sed(int argc, char *argv[]) {
	int i;
	/* 
	 * <옵션 테이블>
	 * [0] : no option
	 * [1] : -b option
	 * [2] : -u option
	 * [3] : -i option
	 * [4] : -e option
	 * [5] : -d option
	 * [6] : -p option
	 * [7] : -s option
	 * [8] : -P option
	 * [9] : -es option
	 * [10] : -is option
	 */
	int option[11] = {0};
	/*
	 * <옵션과 함께 나와야 하는 문자열>
	 * [2] : [USERNAME]
	 * [3] : [STRING]
	 * [4] : [STRING]
	 * [5] : [DEPTH]
	 * [8] : [PATHNAME]
	 * [9] : [STRING]
	 * [10] : [STRING]
	 */
	char *opt_after[11] = {NULL};

	//option[5] == 1이면 이용
	int depth = 0;
    
    //ssu_sed 이름 및 인자들 출력
	for(i = 0; i < argc; i++)
		ssu_printf("arg%d : %s\n", i, argv[i]);	

	//옵션에 따른 테이블 세팅
	//옵션에 따른 opt_after도 세팅
	if(argc == 4)
		option[0]++;
	else if(argc > 4) {
		for(i = 4; i < argc; i++) {
			if(ssu_strcmp(argv[i], "-b") == 0) 
				option[1]++;
			else if(ssu_strcmp(argv[i], "-u") == 0) {
				if(i + 1 >= argc || (i + 1 < argc && argv[i + 1][0] == '-')) {
					ssu_printf("Option Usage: -u [USERNAME]\n");
					return;
				}
				option[2]++;
				if(i + 1 < argc)	
					opt_after[2] = argv[i + 1];
				i++;
			}
			else if(ssu_strcmp(argv[i], "-i") == 0) {
				if(i + 1 >= argc || (i + 1 < argc && argv[i + 1][0] == '-')) {
					ssu_printf("Option Usage: -i [STRING]\n");
					return;	
				}
				option[3]++;
				opt_after[3] = argv[i + 1];
				i++;
			}
			else if(ssu_strcmp(argv[i], "-e") == 0) {
				if(i + 1 >= argc || (i + 1 < argc && argv[i + 1][0] == '-')) {
					ssu_printf("Option Usage: -e [STRING]\n");
					return;	
				}
				option[4]++;
				opt_after[4] = argv[i + 1];
				i++;
			}
			else if(ssu_strcmp(argv[i], "-d") == 0) {
				if(i + 1 >= argc || (i + 1 < argc && argv[i + 1][0] == '-')) {
					ssu_printf("Option Usage: -d [DEPTH]\n");
					return;	
				}
				option[5]++;
				opt_after[5] = argv[i + 1];
				depth = atoi(opt_after[5]);//depth 정수로 변환
				i++;
			}
			else if(ssu_strcmp(argv[i], "-p") == 0)
				option[6]++;
			else if(ssu_strcmp(argv[i], "-s") == 0)
				option[7]++;
			else if(ssu_strcmp(argv[i], "-P") == 0) {
				if(i + 1 >= argc || (i + 1 < argc && argv[i + 1][0] == '-')) {
					ssu_printf("Option Usage: -P [PATHNAME]\n");
					return;	
				}
				option[8]++;
				opt_after[8] = argv[i + 1];
				i++;
			}
			else if(ssu_strcmp(argv[i], "-es") == 0) {
				if(i + 1 >= argc || (i + 1 < argc && argv[i + 1][0] == '-')) {
					ssu_printf("Option Usage: -es [STRING]\n");
					return;	
				}
				option[9]++;
				opt_after[9] = argv[i + 1];
				i++;
			}
			else if(ssu_strcmp(argv[i], "-is") == 0) {
				if(i + 1 >= argc || (i + 1 < argc && argv[i + 1][0] == '-')) {
					ssu_printf("Option Usage: -is [STRING]\n");
					return;	
				}
				option[10]++;
				opt_after[10] = argv[i + 1];
				i++;
			}
			else {
				ssu_printf("ERROR: Such an option doesn't exist.\n");
				print_usage(argv[i]);
				return;
			}
		}
	} 
	else {//조건에 충족하지 못하는 명령의 경우 usage를 출력해줌
		ssu_printf("Usage: ssu_sed [TARGET] [SRC_STR] [DEST_STR] [OPTION]\n");
		return;
	}

    //옵션 테이블을 체크하여 중복 옵션 검사
	for(i = 1; i < 11; i++) 
		if(option[i] >= 2) { 
			ssu_printf("ERROR: Command cannot have same options.\n");
			return;
		}
	
    //디렉토리를 탐색
	search_dir(argv[1], argv[2], argv[3], option, opt_after, depth);
}


//디렉토리를 탐색하고 파일을 처리하는 함수
void search_dir(char *name, char *src, char *dest, int *option, char *opt_after[], int depth) {
	DIR *dir_info;
	struct dirent *entry;
	struct stat file_info;
	int fd;
	int len;
	char path[1000];
	char full_path[10000];
	char full_path2[10000];
	char buf[1000000] ={0};
	struct passwd *pwd;
	int no_upper_lower;
	int i, j;

	if ((dir_info = opendir(name)) == NULL) {//디렉터리가 아닌경우
		realpath(name, full_path);//절대경로를 full_path에 저장
        //regular 파일인 경우
		if(lstat(name, &file_info) >= 0 && S_ISREG(file_info.st_mode)) {
			if((fd = open(name, O_RDWR)) < 0) {
				ssu_printf("%s : failed\n", full_path);
				return;
			}

            //파일 소유권자 체크
			if(option[2] == 1) {	
				if((pwd = getpwuid(file_info.st_uid)) == NULL) {
					ssu_printf("%s : failed\n", full_path);
					return;
				}
				if(ssu_strcmp(pwd->pw_name, opt_after[2]) != 0) {
					ssu_printf("%s : failed\n", full_path);
					return;
				}
			}

            //no_upper_lower를 대소문자 구분 여부의 플래그로 활용
			if(option[7] == 1 || option[9] == 1 || option[10] == 1)
				no_upper_lower = 1;	
			else
				no_upper_lower = 0;	

            //파일 내용을 buf에 저장
			if((len = read(fd, buf, sizeof(buf))) <= 0) {
				ssu_printf("%s : failed\n", full_path);
				return;
			}
			else {
				//파일 내 opt_after[3] 문자열이 존재하지 않으면 실행 안함
				if(option[3] == 1) {
					if(ssu_strstr(buf, opt_after[3], no_upper_lower) == 0) {
						ssu_printf("%s : failed\n", full_path);
						return;
					}
				}
                
                //파일 내 opt_after[10] 문자열이 존재하지 않으면 실행 안함
				if(option[10] == 1) {
					if(ssu_strstr(buf, opt_after[10], no_upper_lower) == 0) {
						ssu_printf("%s : failed\n", full_path);
						return;
					}	
				}

				//파일 내 opt_after[4] 문자열이 존재하면 실행 안함
				if(option[4] == 1) {
					if(ssu_strstr(buf, opt_after[4], no_upper_lower) == 1) {
						ssu_printf("%s : failed\n", full_path);
						return;
					}
				}
                
                //파일 내 opt_after[4] 문자열이 존재하면 실행 안함
				if(option[9] == 1) {
					if(ssu_strstr(buf, opt_after[9], no_upper_lower) == 1) {
						ssu_printf("%s : failed\n", full_path);
						return;
					}
				}
                
                
				if(check_n_change(full_path, buf, src, dest, option)) {//치환 성공
                    //원본을 보존하고 새로운 파일에 변화된 내용 저장
					if(option[8] == 1) {
						ssu_strcpy(full_path2, full_path);
                        //같은 경로에 저장
						for(i = ssu_strlen(full_path) - 1; i >= 0; i--) {
							if(full_path[i] == '/') {
								j = i;	
								break;
							}
						}
						
						for(i = 0; i < ssu_strlen(opt_after[8]); i++)
							full_path[j + 1 + i] = opt_after[8][i];
						full_path[j + 1 + i] = 0;
                        
                        //원본 파일은 수정이 안되었으므로 failed 출력
						ssu_printf("%s : failed\n", full_path2);
                        //다른 파일에 저장
						if((fd = creat(full_path, 0644)) < 0) {
							ssu_printf("%s : failed\n", full_path);
							return;
						}
                        //파일에 수정된 내용 저장
						write(fd, buf, ssu_strlen(buf));
						buf[0] = 0;//buf 비움
						ssu_printf("%s : success\n", full_path);
					}
					else {
                        //원본 파일 삭제했다가 재생성하여 수정
						remove(name);
						if((fd = creat(name, 0644)) < 0) {
							ssu_printf("%s : failed\n", full_path);
							return;
						}
                        //파일에 수정된 내용 저장
						write(fd, buf, ssu_strlen(buf));
						buf[0] = 0;//buf 비움
						ssu_printf("%s : success\n", full_path);
					}
				}
				else {//치환 실패
					ssu_printf("%s : failed\n", full_path);			
				}
			}
		}
		return;
	}

	while((entry = readdir(dir_info)) != NULL) {//
		if (lstat(name, &file_info) >= 0 && S_ISDIR(file_info.st_mode)) {
			if (ssu_strcmp(entry->d_name, ".") == 0 || ssu_strcmp(entry->d_name, "..") == 0)
				continue;
            //path 저장
			len = ssu_sprintf(path, "%s/%s", name, entry->d_name);
			path[len] = 0;
            
            //-d 옵션이 설정된 경우
			if(option[5] == 1) {
                //depth가 0일 때까지 반복
				if(depth == 0) 
					return;
			}
            //-d 옵션이 설정되지 않은 경우 재귀적으로 모든 파일을 찾아감
			search_dir(path, src, dest, option, opt_after, depth - 1);
		}
	}
	closedir(dir_info);
}


//파일 내에서 치환하고자 하는 문자열을 찾아 원하는 문자열로 치환하는 함수
int check_n_change(char *full_path, char *buf, char *src, char *dest, int *option) {
    int i, j;
    int num;
    int start;
    int s_len, d_len, b_len;
	int line;
	int empty = 0;
	int change = 0;
	int no_upper_lower;
    s_len = ssu_strlen(src);
    d_len = ssu_strlen(dest);
    
    start = 0;
	line = 1;
    
    //no_upper_lower를 대소문자 구분 여부의 플래그로 활용
	if(option[7] == 1 || option[9] == 1 || option[10] == 1)
		no_upper_lower = 1;
	else
		no_upper_lower = 0;

	if(option[1] == 1) {//공백 무시하는 경우
		while((num = ssu_strstr3(buf, src, start, ssu_strlen(buf), &empty, no_upper_lower)) != -1) {
			if(change == 0)//치환이 진행되므로 change를 1로 바꿈
				change = 1;
            //s_len + empty와 d_len을 비교하여 buf의 내용을 수정
			if(s_len + empty == d_len) {
				for(i = 0; i < d_len; i++)
					buf[num + i] = dest[i];
			}
			else if(s_len + empty > d_len) {
				b_len = ssu_strlen(buf);
				for(i = 0; i < d_len; i++)
					buf[num + i] = dest[i];

				j = num + d_len;
				for(i = num + s_len + empty; i < b_len; i++) {
					buf[j] = buf[i];
					j++;
				}
				buf[j] = 0;
			}
			else {
				b_len = ssu_strlen(buf);
				j = b_len + (d_len - (s_len + empty)) ;
				buf[j] = 0;
				j--;
				for(i = b_len - 1; i >= num + s_len + empty; i--) {
					buf[j] = buf[i];
					j--;
				}			

				for(i = 0; i < d_len; i++)
					buf[num + i] = dest[i];
			}
            
            //치환된 문자열의 라인 수 출력
			if(option[6] == 1) {
				line = find_line(buf, num + d_len - 1);
				ssu_printf("%s : %d\n", full_path, line);
			}
			start = num + d_len;
			empty = 0;
		}
	}
	else { 
		while((num = ssu_strstr2(buf, src, start, ssu_strlen(buf), no_upper_lower)) != -1) {
			if(change == 0)//치환이 진행되므로 change를 1로 바꿈
				change = 1;
            //s_len과 d_len을 비교하여 buf의 내용을 수정
			if(s_len == d_len) {
				for(i = 0; i < s_len; i++)
					buf[num + i] = dest[i];
			}
			else if(s_len > d_len) {
				for(i = 0; i < d_len; i++)
					buf[num + i] = dest[i];
				b_len = ssu_strlen(buf);

				j = num + d_len;
				for(i = num + s_len; i < b_len; i++) {
					buf[j] = buf[i];
					j++;
				}
				buf[j] = 0;
			}
			else {
				b_len = ssu_strlen(buf);
				j = b_len + (d_len - s_len);
				buf[j] = 0;
				j--;
				for(i = b_len - 1; i >= num + s_len; i--) {
					buf[j] = buf[i];
					j--;
				}			

				for(i = 0; i < d_len; i++)
					buf[num + i] = dest[i];
			}

            //치환된 문자열의 라인 수 출력
			if(option[6] == 1) {
				line = find_line(buf, num + d_len - 1);
				ssu_printf("%s : %d\n", full_path, line);
			}
			start = num + d_len;
		}
	}
	//printf("%s", buf);

	return change;
}

//파일 내에서 치환된 문자열의 라인을 찾아주는 함수
int find_line(char *buf, int index) {
    int line = 1;
    for(int i = 0; i <= index; i++) {
        if(buf[i] == '\n')
            line++;
    }
    
    return line;
}

//usage 출력함수
void print_usage(char *option) {
	ssu_printf("Usage : ssu_sed [TARGET] [SRC_STR] [DEST_STR] [OPTION]\n");
	if(option != NULL) {
		ssu_printf("%s : No such that option\n", option);
		ssu_printf("Option Usage\n");
		ssu_printf(" -b : ignore blanks in files.\n");
		ssu_printf(" -u [USERNAME] : if file's owner is same as [USERNAME], execute.\n");
		ssu_printf(" -i [STRING] : if files include [STRING], execute.\n");
		ssu_printf(" -e [STRING] : if files don't include [STRING], execute.\n");
		ssu_printf(" -d [DEPTH] : search directories until [DEPTH] depth.\n");
		ssu_printf(" -p : if files change, print changed line numbers.\n");
		ssu_printf(" -s : No divide uppercase or lowercase.\n");
		ssu_printf(" -P [PATHNAME] : make [PATHNAME] and save the result in it\n");
		ssu_printf(" -es [STRING] : -s plus -e\n");
		ssu_printf(" -is [STRING] : -s plus -i\n");
	}

}
