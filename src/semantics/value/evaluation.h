#ifndef GCC_SEMANTICS_H
#define GCC_SEMANTICS_H GCC_SEMANTICS_H
#include <ast.h>
#include <lexer.h>
#include <program.h>

long long int evaluate_const_expression_integer(struct AST *expression,struct Translation_Data *translation_data);


unsigned long long int evaluate_integer_constant(struct token *token,struct Translation_Data *translation_data);

unsigned long long int evaluate_literal_integer_dec(struct token *token,struct Translation_Data *translation_data);
unsigned long long int evaluate_literal_integer_hex(struct token *token,struct Translation_Data *translation_data);


long double evaluate_literal_double_dec(struct token *token,struct Translation_Data *translation_data);
long double evaluate_literal_double_hex(struct token *token,struct Translation_Data *translation_data);

char* evaluate_literal_string(struct token *token,struct Translation_Data *translation_data);
wchar_t* evaluate_literal_wide_string(struct token *token,struct Translation_Data *translation_data);

char evaluate_literal_char(struct token *token,struct Translation_Data *translation_data);
wchar_t evaluate_literal_wide_char(struct token *token,struct Translation_Data *translation_data);

wchar_t resolve_char_escape_sequence(struct token *token,struct Translation_Data *translation_data);
#endif
