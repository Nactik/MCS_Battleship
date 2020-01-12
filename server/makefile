all: server

server : server.c serverUtils.o
	gcc server.c serverUtils.o -o server

serverUtils.o : serverUtils.c 
	gcc -c serverUtils.c