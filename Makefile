all:
	gcc -Wno-implicit-function-declaration -c -I h/ -L api/cnaiapi.o client.c
	gcc -I h/ api/cnaiapi.o api/readln.o client.o -o client
	gcc -Wno-implicit-function-declaration -c -I h/ -L api/cnaiapi.o server.c
	gcc -I h/ api/cnaiapi.o api/readln.o server.o -o server

clean:
	rm -f client client.o server server.O
