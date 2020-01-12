all: server

server : server.c serverUtils.o
	gcc server.c serverUtils.o -o server
	mv server ../bin/server

serverUtils.o : serverUtils.c 
	gcc -c serverUtils.c