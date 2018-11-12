all : selecserv cli forkserv
selecserv : selecserv.c unp.h
	gcc selecserv.c -o selecserv
cli : cli.c unp.h
	gcc cli.c  -o cli
forkserv : forkserv.c unp.h
	gcc forkserv.c -o forkserv
clean:
	rm -f selecserv cli
