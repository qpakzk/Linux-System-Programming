ssu_shell: ssu_shell.o ssu_sed.o ssu_stdio.o ssu_string.o
	gcc -o ssu_shell ssu_shell.o ssu_sed.o ssu_stdio.o ssu_string.o
ssu_shell.o: ssu_shell.c
	gcc -c ssu_shell.c ssu_shell.h
ssu_sed.o: ssu_sed.c
	gcc -c ssu_sed.c ssu_sed.h
ssu_stdio.o: ssu_stdio.c
	gcc -c ssu_stdio.c ssu_stdio.h
ss_string.o: ssu_string.c
	gcc -c ssu_string.c ssu_string.h
