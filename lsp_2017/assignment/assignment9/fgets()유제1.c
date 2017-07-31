#include <stdio.h>
#include <stdlib.h>

#define BUFFER_MAX 256

int main(void) {
	char command[BUFFER_MAX];
	char *prompt = "Prompt>>";

	while(1) {
		//표준출력으로 출력
		fputs(prompt, stdout);

		//표준입력으로부터 입력
		if(fgets(command, sizeof(command), stdin) == NULL)
			break;

		//표준입력으로 입력받은 명령을 실행
		system(command);
	}

	fprintf(stdout, "Goodbye...\n");
	fflush(stdout);
	exit(0);
}
