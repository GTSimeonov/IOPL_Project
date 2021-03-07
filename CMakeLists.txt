cmake_minimum_required (VERSION 3.13.4)
project (WONKY)


include_directories(./)

include_directories(backend/js)
include_directories(backend/print)

include_directories(frontend)
include_directories(frontend/lex)
include_directories(frontend/lex/automatas)
include_directories(frontend/parse)
include_directories(frontend/semantics)
include_directories(frontend/semantics/identifiers)
include_directories(frontend/semantics/memory)

include_directories(misc)
include_directories(misc/base_data_structures)

include_directories(program)


set(SOURCES wonky.c 
	frontend/parse/parse_declaration.c
	frontend/parse/parse_expression.c
	frontend/parse/parse_statement.c
	frontend/parse/parse_translation_unit.c
	misc/base_data_structures/map.c
	misc/base_data_structures/queue.c
	misc/base_data_structures/stack.c
	misc/base_data_structures/gcc_string.c
	backend/print/print.c
	frontend/lex/automatas/chonky.c
	frontend/lex/automatas/chonky_jr.c
	frontend/lex/lexer.c
	frontend/lex/preprocessing.c
	frontend/ast.c
	frontend/semantics/memory/location.c
	frontend/semantics/memory/type.c
	frontend/semantics/memory/semantics.c
	frontend/semantics/identifiers/denoted.c
	frontend/semantics/identifiers/scope.c
	frontend/semantics/identifiers/linkage.c
	backend/js/transpile_to_js.c
	program/gcc_error.c
	program/gcc_arguments.c
	program/program.c
   )

add_executable(wonky ${SOURCES})
