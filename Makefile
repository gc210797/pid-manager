pid_manager:
	gcc pid.c main.c -o pid_manager -lpthread
.PHONY: debug clean remove
debug:
	gcc -g pid.c main.c -o pid_manager -lpthread
clean:
	rm pid.c pid.h main.c
uninstall:
	rm pid_manager Makefile
