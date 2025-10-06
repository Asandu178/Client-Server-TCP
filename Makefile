all : client server

server : server.o

server.o : server.c

client : client.o

client.o : client.c

clean:
	rm -rf *.o
	rm -rf client
	rm -rf server