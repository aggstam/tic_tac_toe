all:
	gcc -o client TrilizaC.c
	gcc -o server TrilizaS.c

clean:
	rm -f client server
