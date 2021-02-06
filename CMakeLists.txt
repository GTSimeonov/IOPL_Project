cmake_minimum_required (VERSION 3.13.4)
project (IOPL)


include_directories(misc)
include_directories(parse)
include_directories(semantics)
include_directories(lex)

set(SOURCES main.c 
	parse/parse_declaration.c
	parse/parse_expression.c
	parse/parse_statement.c
	parse/parse_translation_unit.c
	misc/map.c
	misc/queue.c
	misc/stack.c
	misc/print.c
	lex/chonky.c
	lex/chonky_jr.c
	lex/lexer.c
	lex/preprocessing.c
	semantics/ast.c
	semantics/location.c
	semantics/denoted.c
	semantics/type.c
	semantics/program.c
	semantics/scope.c
	semantics/semantics.c
   )

add_executable(wonky ${SOURCES})
