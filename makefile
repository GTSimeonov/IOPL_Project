install:
	gcc -g main.c -o main.exe
run: install
	./main.exe
clear:
	rm ./*.exe &
	rm ./*.temp &
	rm ./*.out
debug:
	gcc -g main.c
	gdbtui a.out
rm: run
rn: run
fuzz: install
	fuzz ./main.exe
