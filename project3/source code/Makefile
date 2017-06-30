CC := gcc
FILENAME1 := ssu_vim
FILENAME2 := ssu_ofm
OPT1 := -o $(FILENAME1) -W -Wall
OPT2 := -o $(FILENAME2) -W -Wall

all: ssu_vim ssu_ofm

ssu_vim: ssu_vim.o
	$(CC) $(OPT1) ssu_vim.o
ssu_vim.o: ssu_vim.c ssu_vim.h
	$(CC) -c ssu_vim.c ssu_vim.h

ssu_ofm: ssu_ofm.o queue.o
	$(CC) $(OPT2) ssu_ofm.o queue.o
ssu_ofm.o: ssu_ofm.c ssu_ofm.h
	$(CC) -c ssu_ofm.c ssu_ofm.h
queue: queue.c queue.h
	$(CC) -c queue.c

clean: 
	rm -f ssu_vim
	rm -f ssu_ofm
	rm -f *.o
	rm -f *.h.gch
