all: client

client : client.c clientUtils.o
	gcc client.c clientUtils.o ../game/gameUtils.o -pthread -o client
	mv client ../bin/client

clientUtils.o : clientUtils.c 
	gcc -c clientUtils.c

clean:
	rm -rf clientUtils.o
	rm -rf ../bin/client