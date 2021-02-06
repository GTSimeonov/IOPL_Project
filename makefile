.PHONY: run clean clear debug rm rn
objects = ast.o chonky.o chonky_jr.o denoted.o lexer.o location.o map.o parse_declaration.o parse_expression.o parse_statement.o parse_translation_unit.o preprocessing.o print.o program.o queue.o scope.o semantics.o stack.o type.o main.o
CC=gcc
CFLAGS="-g"
wonky: ${objects}
	${CC} ${CFLAGS} *.o -o wonky 
run: wonky
	./wonky
main.o: main.c
	${CC} ${CFLAGS} -c main.c -o main.o
ast.o: ast.c ast.h
	${CC} ${CFLAGS} -c ast.c -o ast.o
chonky.o: chonky.c
	${CC} ${CFLAGS} -c chonky.c -o chonky.o
chonky_jr.o: chonky_jr.c 
	${CC} ${CFLAGS} -c chonky_jr.c -o chonky_jr.o
denoted.o: denoted.c denoted.h
	${CC} ${CFLAGS} -c denoted.c -o denoted.o
lexer.o: lexer.c lexer.h
	${CC} ${CFLAGS} -c lexer.c -o lexer.o
location.o: location.c location.h
	${CC} ${CFLAGS} -c location.c -o location.o
parse_declaration.o: parse_declaration.c parse_declaration.h
	${CC} ${CFLAGS} -c parse_declaration.c -o parse_declaration.o
parse_expression.o: parse_expression.c parse_expression.h
	${CC} ${CFLAGS} -c parse_expression.c -o parse_expression.o
parse_statement.o: parse_statement.c parse_statement.h
	${CC} ${CFLAGS} -c parse_statement.c -o parse_statement.o
parse_translation_unit.o: parse_translation_unit.c parse_translation_unit.h
	${CC} ${CFLAGS} -c parse_translation_unit.c -o parse_translation_unit.o
preprocessing.o: preprocessing.c preprocessing.h
	${CC} ${CFLAGS} -c preprocessing.c -o preprocessing.o
print.o: print.c print.h
	${CC} ${CFLAGS} -c print.c -o print.o
program.o: program.c program.h
	${CC} ${CFLAGS} -c program.c -o program.o
scope.o: scope.c scope.h
	${CC} ${CFLAGS} -c scope.c -o scope.o
semantics.o: semantics.c semantics.h
	${CC} ${CFLAGS} -c semantics.c -o semantics.o
test2.o: test2.c test2.h
	${CC} ${CFLAGS} -c test2.c -o test2.o
test.o: test.c test.h
	${CC} ${CFLAGS} -c test.c -o test.o
type.o: type.c type.h
	${CC} ${CFLAGS} -c type.c -o type.o
map.o: map.c map.h
	${CC} ${CFLAGS} -c map.c -o map.o
queue.o: queue.c queue.h
	${CC} ${CFLAGS} -c queue.c -o queue.o
stack.o: stack.c stack.h
	${CC} ${CFLAGS} -c stack.c -o stack.o
clean: clear
clear:
	rm wonky -f &
	rm ./*.temp -f &
	rm ./*.o -f &
	rm ./*.out -f
debug: wonky
	gdbtui wonky
rm: run
rn: run
