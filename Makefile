all:
	gcc -Wno-implicit-function-declaration -c -I h/ -L api/cnaiapi.o TrilizaC.c
	gcc -I h/ api/cnaiapi.o api/readln.o TrilizaC.o -o TrilizaC
	gcc -Wno-implicit-function-declaration -c -I h/ -L api/cnaiapi.o TrilizaS.c
	gcc -I h/ api/cnaiapi.o TrilizaS.o -o TrilizaS

clean:
	rm -f TrilizaC TrilizaC.o TrilizaS TrilizaS.O
