out 	 = bin
lib 	 = src
winlib 	 = $(lib)/libwin.c -lwsock32
linlib 	 = $(lib)/liblin.c
comlib 	 = $(lib)/libcom.c
serv_in	 = server.c

all: linux windows

linux:
	gcc -o $(out)/r.lin -Wall main.c $(linlib) $(comlib)
	@echo done !

windows:
	x86_64-w64-mingw32-gcc -O3 -o $(out)/r.exe -Wall main.c $(winlib) $(comlib)
	@echo done !

server:
	@gcc -o $(out)/server -Wall serv/$(serv_in)
	@echo listening...
	@./$(out)/server

clean:
	rm -rf $(out)/*
	rm readme
	rm caught.key
	@echo projet cleaned !
