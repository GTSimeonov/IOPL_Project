#ifndef WONKY_PARSE_TRANSLATION_H
#define WONKY_PARSE_TRANSLATION_H WONKY_PARSE_TRANSLATION_H
#include <parse_declaration.h>
#include <parse_statement.h>
#include <error.h>

struct AST* parse_translation_unit(struct Translation_Data *translation_data);

#endif
