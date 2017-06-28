#include <stdio.h>
#include <stdlib.h>

struct ssu_id {
	char name[64];
	int id;
};

int main(void) {
	struct ssu_id test1, test2;
	char *fname = "ssu_exam.dat";
	FILE *fp;

	//읽기 전용으로 파일 open
	if((fp = fopen(fname, "w")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}	

	printf("Input ID>> ");
	scanf("%d", &test1.id);
	printf("Input name>> ");
	scanf("%s", test1.name);

	//표준입력으로 부터 입력받은 문자열을 fp가 가리키는 파일에 출력
	if(fwrite(&test1, sizeof(struct ssu_id), 1, fp) != 1) {
		fprintf(stderr, "fwrite error\n");
		exit(1);
	}

	fclose(fp);

	//읽기 전용으로 파일 open
	if((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	//fp가 가리키는 파일 데이터를 test2에 read하기
	if(fread(&test2, sizeof(struct ssu_id), 1, fp) != 1) {
		fprintf(stderr, "fread error\n");
		exit(1);
	}

	printf("\nID	name\n");
	printf("============\n");
	printf("%d	%s\n", test2.id, test2.name);
	fclose(fp);
	exit(0);
}
