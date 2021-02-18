#ifndef GCC_PARSE_TRANSLATION_H
#define GCC_PARSE_TRANSLATION_H GCC_PARSE_TRANSLATION_H
#include <parse_declaration.h>
#include <parse_statement.h>
#include <error.h>

struct AST* parse_translation_unit(struct Translation_Data *translation_data,struct Scope *externs);

#endif
