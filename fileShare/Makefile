all: client server


client: socketUtil.c dataStructures.c client.c
	gcc -o client client.c socketUtil.c dataStructures.c -g -pthread -lm

server: socketUtil.c dataStructures.c server.c
	gcc -o server server.c socketUtil.c dataStructures.c -g -pthread -lm

clean: 
	rm *.o
