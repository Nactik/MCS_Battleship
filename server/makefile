all: server

server : server.c serverUtils.o
	gcc server.c serverUtils.o -o server
	mv server ../bin/server

serverUtils.o : serverUtils.c 
	gcc -c serverUtils.c

clean:
	rm -rf serverUtils.o
	rm -rf ../bin/server