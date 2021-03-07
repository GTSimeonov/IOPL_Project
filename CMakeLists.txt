cmake_minimum_required (VERSION 3.13.4)
project (WONKY)


include_directories(src/)

include_directories(src/backend/js/)
include_directories(src/backend/print)

include_directories(src/frontend)
include_directories(src/frontend/lex)
include_directories(src/frontend/lex/automatas)
include_directories(src/frontend/parse)
include_directories(src/semantics)
include_directories(src/semantics/identifiers)
include_directories(src/semantics/memory)
include_directories(src/semantics/value)

include_directories(src/misc)

include_directories(src/program)


set(SOURCES 
	src/wonky.c 
	src/frontend/parse/parse_declaration.c
	src/frontend/parse/parse_expression.c
	src/frontend/parse/parse_statement.c
	src/frontend/parse/parse_translation_unit.c
	src/misc/map.c
	src/misc/queue.c
	src/misc/stack.c
	src/misc/gcc_string.c
	src/backend/print/print.c
	src/frontend/lex/automatas/chonky.c
	src/frontend/lex/automatas/chonky_jr.c
	src/frontend/lex/lexer.c
	src/frontend/lex/preprocessing.c
	src/semantics/ast.c
	src/semantics/memory/location.c
	src/semantics/memory/object.c
	src/semantics/value/type.c
	src/semantics/value/evaluation.c
	src/semantics/identifiers/denoted.c
	src/semantics/identifiers/scope.c
	src/semantics/identifiers/linkage.c
	src/backend/js/transpile_to_js.c
	src/program/gcc_error.c
	src/program/gcc_arguments.c
	src/program/program.c
   )

add_executable(wonky ${SOURCES})
