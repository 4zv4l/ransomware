out = bin
lib = src
winlib = $(lib)/libwin.c
linlib = $(lib)/liblin.c

all: linux windows

linux:
	gcc -o $(out)/r.lin -Wall main.c $(linlib)
	@echo done !

windows:
	x86_64-w64-mingw32-gcc -o $(out)/r.exe -Wall main.c $(winlib)
	@echo done !

clean:
	rm -rf $(out)/*
	@echo projet cleaned !
