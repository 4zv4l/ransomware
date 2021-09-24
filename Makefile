out = bin
lib = src
winlib = $(lib)/libwin.c
linlib = $(lib)/liblin.c
comlib = $(lib)/libcom.c

all: linux windows

linux:
	gcc -o $(out)/r.lin -Wall main.c $(linlib) $(comlib)
	@echo done !

windows:
	x86_64-w64-mingw32-gcc -o $(out)/r.exe -Wall main.c $(winlib) $(comlib)
	@echo done !

clean:
	rm -rf $(out)/*
	@echo projet cleaned !
