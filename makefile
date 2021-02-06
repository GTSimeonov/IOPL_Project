install: ast.o chonky.o chonky_jr.o denoted.o lexer.o location.o map.o parse_declaration.o parse_expression.o parse_statement.o parse_translation_unit.o preprocessing.o print.o program.o queue.o scope.o semantics.o stack.o type.o main.o
	gcc -g *.o -o wonky 
run: install
	./main.exe

main.o: main.c
	gcc -c main.c -o main.o
ast.o: ast.c ast.h
	gcc -c ast.c -o ast.o
chonky.o: chonky.c
	gcc -c chonky.c -o chonky.o
chonky_jr.o: chonky_jr.c 
	gcc -c chonky_jr.c -o chonky_jr.o
denoted.o: denoted.c denoted.h
	gcc -c denoted.c -o denoted.o
lexer.o: lexer.c lexer.h
	gcc -c lexer.c -o lexer.o
location.o: location.c location.h
	gcc -c location.c -o location.o
parse_declaration.o: parse_declaration.c parse_declaration.h
	gcc -c parse_declaration.c -o parse_declaration.o
parse_expression.o: parse_expression.c parse_expression.h
	gcc -c parse_expression.c -o parse_expression.o
parse_statement.o: parse_statement.c parse_statement.h
	gcc -c parse_statement.c -o parse_statement.o
parse_translation_unit.o: parse_translation_unit.c parse_translation_unit.h
	gcc -c parse_translation_unit.c -o parse_translation_unit.o
preprocessing.o: preprocessing.c preprocessing.h
	gcc -c preprocessing.c -o preprocessing.o
print.o: print.c print.h
	gcc -c print.c -o print.o
program.o: program.c program.h
	gcc -c program.c -o program.o
scope.o: scope.c scope.h
	gcc -c scope.c -o scope.o
semantics.o: semantics.c semantics.h
	gcc -c semantics.c -o semantics.o
test2.o: test2.c test2.h
	gcc -c test2.c -o test2.o
test.o: test.c test.h
	gcc -c test.c -o test.o
type.o: type.c type.h
	gcc -c type.c -o type.o
map.o: map.c map.h
	gcc -c map.c -o map.o
queue.o: queue.c queue.h
	gcc -c queue.c -o queue.o
stack.o: stack.c stack.h
	gcc -c stack.c -o stack.o

clear:
	rm ./*.exe -f &
	rm ./*.temp -f &
	rm ./*.o -f &
	rm ./*.out -f
debug:
	gcc -g main.c
	gdbtui a.out
rm: run
rn: run
fuzz: install
	fuzz ./main.exe
