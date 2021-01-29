#ifndef GCC_PARSE_TRANSLATION_H
#define GCC_PARSE_TRANSLATION_H GCC_PARSE_TRANSLATION_H

struct AST* parse_translation_unit_step(struct Queue *tokens,struct Scope *scope);
struct AST* parse_translation_unit(struct Queue *tokens,struct Scope *scope);

#endif
